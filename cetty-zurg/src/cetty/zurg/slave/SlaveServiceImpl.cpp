
#include <cetty/zurg/slave/SlaveServiceImpl.h>

#include <cetty/zurg/slave/Process.h>
#include <cetty/zurg/slave/ApplicationManager.h>
#include <cetty/zurg/slave/ProcessManager.h>
#include <cetty/zurg/slave/GetHardwareTask.h>

#include <boost/weak_ptr.hpp>

#include <cetty/util/SmallFile.h>
#include <cetty/logging/LoggerHelper.h>

namespace cetty {
namespace zurg {
namespace slave {

using namespace cetty::util;
using namespace cetty::zurg::proto;

int g_tempFileCount = 0;

std::string writeTempFile(StringPiece prefix, StringPiece content) {
    char buf[256];
    ::snprintf(buf, sizeof buf, "/tmp/%s-runScript-%s-%d-%05d-XXXXXX",
               prefix.data(),
               ProcessInfo::startTime().toString().c_str(),
               ::getpid(),
               ++g_tempFileCount);

    int tempfd = ::mkostemp(buf, O_CLOEXEC);
    ssize_t n = ::pwrite(tempfd, content.data(), content.size(), 0);
    ::fchmod(tempfd, 0755);
    ::close(tempfd);

    return n == content.size() ? buf : "";
}

void parseMd5sum(const std::string& lines, std::map<StringPiece, StringPiece>* md5sums) {
    const char* p = lines.c_str();
    size_t nl = 0;

    while (*p) {
        StringPiece md5(p, 32);
        nl = lines.find('\n', nl);
        assert(nl != std::string::npos);

        StringPiece file(p + 34, static_cast<int>(lines.c_str() + nl - p - 34));
        (*md5sums)[file] = md5;

        p = lines.c_str() + nl + 1;
        ++nl;
    }
}

SlaveServiceImpl::SlaveServiceImpl(const EventLoopPtr& loop, int zombieInterval)
    : loop(loop),
      children(new ProcessManager(loop, zombieInterval)),
      apps(new ApplicationManager(loop, children.get())) {
}

SlaveServiceImpl::~SlaveServiceImpl() {
}

void SlaveServiceImpl::start() {
    children->start();
}

void SlaveServiceImpl::getHardware(const ConstGetHardwareRequestPtr& request,
                                   const GetHardwareResponsePtr& response,
                                   const DoneCallback& done) {

    LOG_INFO << "SlaveServiceImpl::getHardware - lshw = " << request->lshw();

    GetHardwareTaskPtr task(new GetHardwareTask(request, done));
    task->start(this);
}

void SlaveServiceImpl::getFileContent(const ConstGetFileContentRequestPtr& request,
                                      const GetFileContentResponsePtr& response,
                                      const DoneCallback& done) {
    LOG_INFO << "SlaveServiceImpl::getFileContent - " << request->file_name()
             << " maxSize = " << request->max_size();

    int64_t file_size = 0;
    int64_t modify_time = 0;
    int64_t create_time = 0;
    int err = SmallFile::readFile(request->file_name(),
                                  request->max_size(),
                                  response->mutable_content(),
                                  &file_size,
                                  &modify_time,
                                  &create_time);
    response->set_error_code(err);
    response->set_file_size(file_size);
    response->set_modify_time(modify_time);
    response->set_create_time(create_time);

    LOG_DEBUG << "SlaveServiceImpl::getFileContent - err " << err;
    done(response);
}

void SlaveServiceImpl::getFileChecksum(const ConstGetFileChecksumRequestPtr& request,
                                       const GetFileChecksumResponsePtr& response,
                                       const DoneCallback& done) {
    DLOG_DEBUG ;

    if (request->files_size() > 0) {
        RunCommandRequestPtr runCommandReq(new RunCommandRequest);
        runCommandReq->set_command("md5sum");

        for (int i = 0; i < request->files_size(); ++i) {
            assert(request->files(i).find('\n') == std::string::npos);
            runCommandReq->add_args(request->files(i));
        }

        runCommand(runCommandReq, NULL,
                   boost::bind(&SlaveServiceImpl::getFileChecksumDone, this, request, _1, done));
    }
    else {
        GetFileChecksumResponse response;
        done(&response);
    }
}

void SlaveServiceImpl::getFileChecksumDone(const RunCommandRequestPtr& request,
        const RunCommandResponsePtr& response,
        const GetFileChecksumResponsePtr& checksumResponse,
        const DoneCallback& done) {

    const std::string& lines = response->std_output();
    std::map<StringPiece, StringPiece> md5sums;
    parseMd5sum(lines, &md5sums);

    GetFileChecksumResponse response;

    for (int i = 0; i < request->files_size(); ++i) {
        md5sums[request->files(i)].copy_to(response.add_md5sums());
    }

    done(checksumResponse);
}

void SlaveServiceImpl::runCommand(const ConstRunCommandRequestPtr& request,
                                  const RunCommandResponsePtr& response,
                                  const DoneCallback& done) {
    LOG_INFO << "SlaveServiceImpl::runCommand - " << request->command();

    ProcessPtr process(new Process(loop, request, done));
    int err = 12; // ENOMEM;

    try {
        err = process->start();
    }
    catch (...) {
    }

    if (err) {
        response->set_error_code(err);
        done(response);
    }
    else {
        children->runAtExit(process->pid(),  // bind strong ptr
                            boost::bind(&Process::onCommandExit, process, _1, _2));

        boost::weak_ptr<Process> weakProcessPtr(process);
        TimeoutPtr timerId = loop_->runAfter(request->timeout(),
                                             boost::bind(&Process::onTimeoutWeak, weakProcessPtr));

        process->setTimerId(timerId);
    }
}

void SlaveServiceImpl::runScript(const ConstRunScriptRequestPtr& request,
                                 const RunCommandResponsePtr& response,
                                 const DoneCallback& done) {
    RunCommandRequestPtr runCommandReq(new RunCommandRequest);

    std::string scriptFile = writeTempFile(SlaveApp::instance().name(), request->script());

    if (!scriptFile.empty()) {
        LOG_INFO << "runScript - write to " << scriptFile;

        // FIXME: interpreter
        runCommandReq->set_command(scriptFile);

        // FIXME: others
        runCommand(runCommandReq, NULL, done);
    }
    else {
        LOG_ERROR << "runScript - failed to write script file";
        // FIXME: done
    }
}

void SlaveServiceImpl::addApplication(const ConstAddApplicationRequestPtr& request,
                                      const AddApplicationResponsePtr& response,
                                      const DoneCallback& done) {
    apps->add(request, done);
}

void SlaveServiceImpl::startApplications(const ConstStartApplicationsRequestPtr& request,
        const StartApplicationsResponsePtr& response,
        const DoneCallback& done) {
    apps->start(request, done);
}

void SlaveServiceImpl::stopApplication(const ConstStopApplicationRequestPtr& request,
                                       const StopApplicationResponsePtr& response,
                                       const DoneCallback& done) {
    apps->stop(request, response, done);
}

void SlaveServiceImpl::getApplications(const ConstGetApplicationsRequestPtr& request,
                                       const GetApplicationsResponsePtr& response,
                                       const DoneCallback& done) {
    apps->get(request, response, done);
}

void SlaveServiceImpl::listApplications(const ConstListApplicationsRequestPtr& request,
                                        const ListApplicationsResponsePtr& response,
                                        const DoneCallback& done) {
    apps->list(request, response, done);
}

void SlaveServiceImpl::removeApplications(const ConstRemoveApplicationsRequestPtr& request,
        const RemoveApplicationsResponsePtr& response,
        const DoneCallback& done) {
    apps->remove(request, response, done);
}

}
}
}