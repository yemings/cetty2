#if !defined(CETTY_SERVICE_BUILDER_SERVERBUILDERCONFIG_H)
#define CETTY_SERVICE_BUILDER_SERVERBUILDERCONFIG_H

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
#include <cetty/config/ConfigObject.h>
#include <cetty/config/ConfigDescriptor.h>

namespace cetty {
namespace service {
namespace builder {

using namespace cetty::config;

class ServerBuilderConfig : public cetty::config::ConfigObject {
public:
    class Server : public ConfigObject {
    public:
        int port;
        std::string host;
        std::string name;

    public:
        Server();
        ConfigObject* create() const { return new Server; }
    };

    class Limit : public ConfigObject {
    public:
        int maxConcurrentRequests;

    public:
        Limit();
        ConfigObject* create() const { return new Limit; }
    };

    struct Timeout : public ConfigObject {
    public:
        int hostConnectionMaxIdleTime;

    public:
        ConfigObject* create() const { return new Timeout; }
        Timeout();
    };

public:
    int deamonize;
    std::string pidfile;
    std::vector<Server*> servers;

    int parentThreadCount;
    int childThreadCount;
    int backlog;
    std::string logger;
    std::string logLevel;

    Limit* limit;
    Timeout* timeout;
	
public:
    ServerBuilderConfig();

    ~ServerBuilderConfig() {
        if (limit) delete limit;
        if (timeout) delete timeout;
    }

    ConfigObject* create() const { return new ServerBuilderConfig; }
};

}
}
}

#endif //#if !defined(CETTY_SERVICE_BUILDER_SERVERBUILDERCONFIG_H)

// Local Variables:
// mode: c++
// End:
