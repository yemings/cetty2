/*
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
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

#include <cetty/channel/socket/asio/AsioServerSocketChannelConfig.h>

#include <boost/any.hpp>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>
#include <cetty/channel/ChannelException.h>
#include <cetty/util/Exception.h>
#include <cetty/util/internal/ConversionUtil.h>

namespace cetty {
namespace channel {
namespace socket {
namespace asio {

using namespace cetty::channel;
using namespace cetty::channel::socket;
using namespace cetty::util;

AsioServerSocketChannelConfig::AsioServerSocketChannelConfig(
    boost::asio::ip::tcp::acceptor& acceptor)
    : reuseAddress(true), receiveBufferSize(0), backlog(0), acceptor(acceptor) {
}

AsioServerSocketChannelConfig::~AsioServerSocketChannelConfig() {
}

bool AsioServerSocketChannelConfig::setOption(const ChannelOption& option,
        const ChannelOption::Variant& value) {
    if (DefaultChannelConfig::setOption(option, value)) {
        return true;
    }

    if (option == ChannelOption::CO_SO_RCVBUF) {
        setReceiveBufferSize(boost::get<int>(value));
    }
    else if (option == ChannelOption::CO_SO_REUSEADDR) {
        setReuseAddress(boost::get<bool>(value));
    }
    else if (option == ChannelOption::CO_SO_BACKLOG) {
        setBacklog(internal::ConversionUtil::toInt(value));
    }
    else {
        return false;
    }

    return true;
}

const boost::optional<bool>& AsioServerSocketChannelConfig::isReuseAddress() const {
    if (reuseAddress) {
        return reuseAddress;
    }

    if (this->acceptor.is_open()) {
        reuseAddress = isReuseAddress(this->acceptor);
    }

    return reuseAddress;
}

bool AsioServerSocketChannelConfig::isReuseAddress(tcp::acceptor& acceptor) const {
    try {
        boost::asio::ip::tcp::acceptor::reuse_address option;
        acceptor.get_option(option);
        return option.value();
    }
    catch (const boost::system::system_error& e) {
        throw ChannelException(e.what(), e.code().value());
    }
}

void AsioServerSocketChannelConfig::setReuseAddress(bool reuseAddress) {
    this->reuseAddress = reuseAddress;

    if (this->acceptor.is_open()) {
        setReuseAddress(this->acceptor);
    }
}

void AsioServerSocketChannelConfig::setReuseAddress(tcp::acceptor& acceptor) {
    if (reuseAddress) {
        boost::system::error_code error;
        boost::asio::ip::tcp::acceptor::reuse_address option(*reuseAddress);
        acceptor.set_option(option, error);

        if (error) {
            ChannelException(error.message(), error.value());
        }
    }
}

const boost::optional<int>& AsioServerSocketChannelConfig::getReceiveBufferSize() const {
    if (receiveBufferSize) {
        return receiveBufferSize;
    }

    if (this->acceptor.is_open()) {
        this->receiveBufferSize = getReceiveBufferSize(this->acceptor);
    }

    return receiveBufferSize;
}

int AsioServerSocketChannelConfig::getReceiveBufferSize(tcp::acceptor& acceptor) const {
    try {
        boost::asio::ip::tcp::acceptor::receive_buffer_size option;
        acceptor.get_option(option);
        return option.value();
    }
    catch (const boost::system::system_error& e) {
        throw ChannelException(e.what(), e.code().value());
    }
}


void AsioServerSocketChannelConfig::setReceiveBufferSize(int receiveBufferSize) {
    if (receiveBufferSize > 0) {
        this->receiveBufferSize = receiveBufferSize;

        if (this->acceptor.is_open()) {
            setReceiveBufferSize(this->acceptor);
        }
    }
}

void AsioServerSocketChannelConfig::setReceiveBufferSize(tcp::acceptor& acceptor) {
    if (receiveBufferSize) {
        boost::system::error_code error;
        boost::asio::ip::tcp::acceptor::receive_buffer_size option(*receiveBufferSize);
        acceptor.set_option(option, error);

        if (error) {
            throw ChannelException(error.message(), error.value());
        }
    }
}

void AsioServerSocketChannelConfig::setPerformancePreferences(int connectionTime, int latency, int bandwidth) {
    //socket.setPerformancePreferences(connectionTime, latency, bandwidth);
}

const boost::optional<int>& AsioServerSocketChannelConfig::getBacklog() const {
    return this->backlog;
}

void AsioServerSocketChannelConfig::setBacklog(int backlog) {
    if (backlog < 0) {
        throw InvalidArgumentException("backlog: is less then zero");
    }

    this->backlog = backlog;
}

}
}
}
}