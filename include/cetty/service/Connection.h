#if !defined(CETTY_SERVICE_CONNECTION_H)
#define CETTY_SERVICE_CONNECTION_H

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

#include <string>
#include <vector>
#include <cetty/util/StringUtil.h>

namespace cetty {
namespace service {

using namespace cetty::util;

struct Connection {
    int port;
    int limited;
    std::string host;

    // format like :  host:port:limited
    Connection(const std::string& connection)
        : port(),
          limited() {
        std::vector<std::string> segments;
        StringUtil::split(connection, ':', &segments);

        if (!segments.empty()) {
            host = segments.at(0);
            port = StringUtil::strto32(segments.at(1));

            if (segments.size() > 2) {
                limited = StringUtil::strto32(segments.at(2));
            }
        }
    }

    Connection(const std::string& host, int port, int limited)
        : port(port), limited(limited), host(host) {}

    Connection(int port, int limited)
        : port(port), limited(limited) {}
};

typedef std::vector<Connection> Connections;

}
}

#endif //#if !defined(CETTY_SERVICE_CONNECTION_H)

// Local Variables:
// mode: c++
// End:
