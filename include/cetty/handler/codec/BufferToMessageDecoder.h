#if !defined(CETTY_HANDLER_CODEC_BUFFERTOMESSAGEDECODER_H)
#define CETTY_HANDLER_CODEC_BUFFERTOMESSAGEDECODER_H
/*
 * Copyright 2012 The Netty Project
 *
 * The Netty Project licenses this file to you under the Apache License,
 * version 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
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

#include <cetty/buffer/ChannelBuffer.h>
#include <cetty/channel/ChannelMessageHandlerContext.h>
#include <cetty/handler/codec/DecoderException.h>
#include <cetty/util/Exception.h>

namespace cetty {
namespace handler {
namespace codec {

using namespace cetty::buffer;
using namespace cetty::util;

template<typename InboundOut>
class BufferToMessageDecoder : private boost::noncopyable {
public:
    typedef BufferToMessageDecoder<InboundOut> Self;
    typedef boost::shared_ptr<Self> Ptr;

    typedef ChannelMessageContainer<InboundOut, MESSAGE_BLOCK> InboundInContainer;
    typedef typename InboundInContainer::MessageQueue MessageQueue;
    
    typedef ChannelMessageHandlerContext<
        BufferToMessageDecoder<InboundOut>,
        ChannelBufferPtr,
        InboundOut,
        VoidMessage,
        VoidMessage,
        ChannelBufferContainer,
        InboundInContainer,
        VoidMessageContainer,
        VoidMessageContainer> Context;

public:
    virtual void registerTo(Context& ctx) {
        context_ = &ctx;

        ctx.setChannelMessageUpdatedCallback(boost::bind(
            &Self::messageUpdated,
            this,
            _1));

        ctx.setChannelInactiveCallback(boost::bind(
            &Self::channelInactive,
            this,
            _1));
    }

    void messageUpdated(ChannelHandlerContext& ctx) {
        callDecode(ctx);
    }

    void channelInactive(ChannelHandlerContext& ctx) {
#if 0
        const ChannelBufferPtr& in = getInboundChannelBuffer();

        if (in && in->readable()) {
            callDecode(ctx);

            try {
                if (inboundTransfer.unfoldAndAdd(decodeLast(ctx, in))) {
                    in->discardReadBytes();
                    ctx.fireMessageUpdated();
                }
            }
            catch (const CodecException& e) {
                ctx.fireExceptionCaught(e);
            }
            catch (const std::exception& e) {
                ctx.fireExceptionCaught(DecoderException(e.what()));
            }
        }

        ctx.fireChannelInactive();
#endif
    }

    virtual InboundOut decode(ChannelHandlerContext& ctx,
                               const ChannelBufferPtr& in) = 0;

    InboundOut decodeLast(ChannelHandlerContext& ctx,
                                   const ChannelBufferPtr& in) {
        return decode(ctx, in);
    }

#if 0
    /**
     * Replace this decoder in the {@link ChannelPipeline} with the given handler.
     * All remaining bytes in the inbound buffer will be forwarded to the new handler's
     * inbound buffer.
     */
    void replace(const std::string& newHandlerName, ChannelInboundByteHandler newHandler) {
        if (!ctx.executor().inEventLoop()) {
            throw new IllegalStateException("not in event loop");
        }

        // We do not use ChannelPipeline.replace() here so that the current context points
        // the new handler.
        ctx.pipeline().addAfter(ctx.name(), newHandlerName, newHandler);

        ByteBuf in = ctx.inboundByteBuffer();

        try {
            if (in->readable()) {
                ctx.nextInboundByteBuffer().writeBytes(ctx.inboundByteBuffer());
                ctx.fireMessageUpdated();
            }
        } finally {

            ctx.pipeline().remove(this);
        }
    }
#endif

protected:
    void callDecode(ChannelHandlerContext& ctx) {
        const ChannelBufferPtr& in =
            context_->inboundContainer()->getMessages();

        bool decoded = false;

        for (;;) {
            try {
                int oldInputLength = in->readableBytes();
                InboundOut o = decode(ctx, in);

                if (!o) {
                    if (oldInputLength == in->readableBytes()) {
                        break;
                    }
                    else {
                        continue;
                    }
                }
                else {
                    if (oldInputLength == in->readableBytes()) {
                        throw IllegalStateException(
                            "decode() did not read anything but decoded a message.");
                    }
                }

                if (context_->inboundTransfer()->unfoldAndAdd(o)) {
                    decoded = true;
                }
                else {
                    break;
                }
            }
            catch (const CodecException& e) {
                if (decoded) {
                    decoded = false;
                    in->discardReadBytes();
                    ctx.fireMessageUpdated();
                }

                ctx.fireExceptionCaught(e);
            }
            catch (const std::exception& e) {
                if (decoded) {
                    decoded = false;
                    in->discardReadBytes();
                    ctx.fireMessageUpdated();
                }

                ctx.fireExceptionCaught(DecoderException(e.what()));
            }
        }

        if (decoded) {
            in->discardReadBytes();
            ctx.fireMessageUpdated();
        }
    }

protected:
    Context* context_;
};

}
}
}

#endif //#if !defined(CETTY_HANDLER_CODEC_BUFFERTOMESSAGEDECODER_H)

// Local Variables:
// mode: c++
// End:
