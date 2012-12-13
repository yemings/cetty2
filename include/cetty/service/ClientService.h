#if !defined(CETTY_SERVICE_CLIENTSERVICE_H)
#define CETTY_SERVICE_CLIENTSERVICE_H

/*
 * Copyright (c) 2010-2012 frankee zhou (frankee.zhou at gmail dot com)
 *
 * Distributed under under the Apache License, version 2.0 (the "License").
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <vector>
#include <cetty/util/ReferenceCounter.h>
#include <cetty/channel/Channel.h>
#include <cetty/channel/ChannelConfig.h>

#include <cetty/service/ServiceFuture.h>
#include <cetty/service/ClientServicePtr.h>
#include <cetty/service/ClientServiceDispatcher.h>

namespace cetty {
namespace service {

using namespace cetty::channel;

template<typename Request, typename Response>
class ClientService : public cetty::channel::Channel {
public:
    typedef ClientService<Request, Response> Self;

    typedef ClientServiceDispatcher<Self,
            Request,
            Response> Dispatcher;

public:
    ClientService(const EventLoopPtr& eventLoop,
                  const Initializer& initializer,
                  const Connections& connections)
        : eventLoop_(eventLoop),
          initializer_(initializer),
          connections_(connections) {
    }

    virtual ~ClientService() {}

    virtual bool isOpen() const { return true; }
    virtual bool isActive() const { return true; }

private:
    void doBind(const SocketAddress& localAddress) {}
    void doDisconnect() {}
    void doClose() {}

    void doInitialize() {
        pipeline().setHead<Dispatcher::HandlerPtr>("dispatcher",
                Dispatcher::HandlerPtr(new Dispatcher(eventLoop_,
                                       connections_,
                                       initializer_)));
    }

private:
    EventLoopPtr eventLoop_;
    Initializer initializer_;
    Connections connections_;
};

template<typename ReqT, typename RepT>
void callMethod(const ChannelPtr& channel,
                const ReqT& request,
                const boost::intrusive_ptr<ServiceFuture<RepT> >& future) {
    if (channel) {
        boost::intrusive_ptr<OutstandingCall<ReqT, RepT> > outstanding(
            new OutstandingCall<ReqT, RepT>(request, future));
        channel->writeMessage(outstanding);
    }
}

}
}

#endif //#if !defined(CETTY_SERVICE_CLIENTSERVICE_H)

// Local Variables:
// mode: c++
// End:
