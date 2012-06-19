#if !defined(CETTY_PROTOBUF_HANDLER_PROTOBUFRPCDECODER_H)
#define CETTY_PROTOBUF_HANDLER_PROTOBUFRPCDECODER_H

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

#include <cetty/handler/codec/oneone/OneToOneDecoder.h>

namespace cetty {
namespace protobuf {
namespace proto {
class RpcMessage;
}
}
}

namespace cetty {
namespace protobuf {
namespace handler {

using namespace cetty::channel;
using namespace cetty::protobuf::proto;

class ProtobufRpcDecoder : public cetty::handler::codec::oneone::OneToOneDecoder {
public:
    ProtobufRpcDecoder();
    virtual ~ProtobufRpcDecoder();

    virtual ChannelHandlerPtr clone();
    virtual std::string toString() const;

protected:
    virtual ChannelMessage decode(ChannelHandlerContext& ctx,
                                  const ChannelPtr& channel,
                                  const ChannelMessage& msg);


private:
    int decode(const ChannelBufferPtr& buffer, RpcMessage* message);

};

}
}
}

#endif //#if !defined(CETTY_PROTOBUF_HANDLER_PROTOBUFRPCDECODER_H)

// Local Variables:
// mode: c++
// End:
