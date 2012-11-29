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

#include <cetty/channel/asio/AsioIpAddressImplFactory.h>
#include <cetty/channel/asio/AsioIpAddressImpl.h>

namespace cetty {
namespace channel {
namespace asio {

using namespace cetty::channel;

IpAddressImpl* AsioIpAddressImplFactory::create(int family) {
    return new AsioIpAddressImpl(family);
}

IpAddressImpl* AsioIpAddressImplFactory::create(const std::string& addr) {
    return new AsioIpAddressImpl(addr);
}

IpAddressImpl* AsioIpAddressImplFactory::create(const std::string& addr,int family) {
    return new AsioIpAddressImpl(addr, family);
}

}
}
}
