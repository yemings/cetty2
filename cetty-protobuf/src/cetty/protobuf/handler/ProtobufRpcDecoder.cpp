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

#include <cetty/protobuf/handler/ProtobufRpcDecoder.h>

#include <cetty/buffer/ChannelBuffer.h>
#include <cetty/protobuf/proto/rpc.pb.h>
#include <cetty/protobuf/ProtobufServiceRegister.h>

namespace cetty {
namespace protobuf {
namespace handler {

using namespace cetty::buffer;
using namespace cetty::channel;
using namespace cetty::protobuf;
using namespace cetty::protobuf::proto;

ChannelMessage ProtobufRpcDecoder::decode(ChannelHandlerContext& ctx,
        const ChannelPtr& channel,
        const ChannelMessage& msg) {

    ChannelBufferPtr buffer = msg.smartPointer<ChannelBuffer>();

    if (buffer) {
        RpcMessage* message = new RpcMessage;

        if (!decode(buffer, message)) {
            const google::protobuf::Message* prototype = NULL;

            if (message->type() == REQUEST) {
                prototype = ProtobufServiceRegister::instance().getRequestPrototype(
                                message->service(), message->method());
            }
            else if (message->type() == RESPONSE) {
                prototype = ProtobufServiceRegister::instance().getResponsePrototype(
                                message->service(), message->method());
            }

            google::protobuf::Message* payload = prototype->New();
            Array arry;
            buffer->readableBytes(&arry);
            payload->ParseFromArray(arry.data(), arry.length());
        }
    }
}

int ProtobufRpcDecoder::decode(const ChannelBufferPtr& buffer, RpcMessage* message) {

}

}
}
}