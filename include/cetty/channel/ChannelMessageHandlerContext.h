#if !defined(CETTY_CHANNEL_CHANNELMESSAGEHANDLERCONTEXT_H)
#define CETTY_CHANNEL_CHANNELMESSAGEHANDLERCONTEXT_H

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

#include <cetty/channel/ChannelHandlerContext.h>
#include <cetty/channel/ChannelHandlerWrapper.h>
#include <cetty/channel/ChannelMessageTransfer.h>

namespace cetty {
namespace channel {

template<typename H,
         typename InboundIn,
         typename InboundOut,
         typename OutboundIn,
         typename OutboundOut,
         typename InboundInContainer,
         typename InboundOutContainer,
         typename OutboundInContainer,
         typename OutboundOutContainer>
class ChannelMessageHandlerContext : public ChannelHandlerContext {
public:
    typedef ChannelMessageHandlerContext<H,
            InboundIn,
            InboundOut,
            OutboundIn,
            OutboundOut,
            InboundInContainer,
            InboundOutContainer,
            OutboundInContainer,
            OutboundOutContainer> Context;

    typedef typename ChannelHandlerWrapper<H>::Handler Handler;
    typedef typename ChannelHandlerWrapper<H>::HandlerPtr HandlerPtr;
    typedef typename ChannelHandlerWrapper<H>::StoredHandlerPtr StoredHandlerPtr;

    typedef InboundInContainer InboundContainer;
    typedef InboundOutContainer NextInboundContainer;
    typedef OutboundInContainer OutboundContainer;
    typedef OutboundOutContainer NextOutboundContainer;

    typedef ChannelMessageTransfer<InboundOut,
            InboundOutContainer,
            TRANSFER_INBOUND> InboundTransfer;

    typedef ChannelMessageTransfer<OutboundOut,
            OutboundOutContainer,
            TRANSFER_OUTBOUND> OutboundTransfer;

    /**
     * Handler是能够注册callback及functor的对象（即这些callback及funcotor的容器）
     *
     * 默认情况下，当需要把Handler至于Context容器内时，Context会自动调用Handler的registerTo的方法，
     * 即，Handler需要有一个registerTo的成员方法。
     * 当提供的Handler没有RegisterTo的成员方法，或是该Handler需要同时置于两个即两个以上的Context，且不同的Context注册不同的信号处理器，
     * 可以在初始化时提供注册的callback，由Context调用该callback。
     * 采用这样的方式，Handler可以是用户指定的任意的对象，并不需要强制继承任何预定的ChannelHandler（like netty）
     *
     * Get called when the Context initialized with the handler, handler should
     * set the callbacks or the functors to the context.
     *
     * <code>
     * void registerFunctions(const HandlerPtr& handler, Context& ctx) {
     *
     * }
     *
     * </code>
     */
    typedef boost::function<void (HandlerPtr const&, Context&)> RegisterCallback;

public:
    ChannelMessageHandlerContext(const std::string& name,
                                 const HandlerPtr& handler)
        : ChannelHandlerContext(name),
          handler_(handler),
          inboundTransfer_(*this),
          outboundTransfer_(*this) {
        if (handler) {
            handler->registerTo(*this);
        }
    }

    ChannelMessageHandlerContext(const std::string& name,
                                 const HandlerPtr& handler,
                                 const EventLoopPtr& eventLoop)
        : ChannelHandlerContext(name, eventLoop),
          inboundTransfer_(*this),
          outboundTransfer_(*this) {
        if (handler) {
            handler->registerTo(*this);
        }
    }

    ChannelMessageHandlerContext(const std::string& name,
                                 const HandlerPtr& handler,
                                 const RegisterCallback& registerCallback)
        : ChannelHandlerContext(name),
          handler_(handler),
          inboundTransfer_(*this),
          outboundTransfer_(*this) {
        if (handler && registerCallback) {
            registerCallback(handler, *this);
        }
    }

    ChannelMessageHandlerContext(const std::string& name,
                                 const HandlerPtr& handler,
                                 const RegisterCallback& registerCallback,
                                 const EventLoopPtr& eventLoop)
        : ChannelHandlerContext(name, eventLoop),
          handler_(handler),
          inboundTransfer_(*this),
          outboundTransfer_(*this) {
        if (handler && registerCallback) {
            registerCallback(handler, *this);
        }
    }

    InboundContainer* inboundContainer() {
        return &inboundContainer_;
    }

    OutboundContainer* outboundContainer() {
        return &outboundContainer_;
    }

    InboundTransfer* inboundTransfer() {
        return &inboundTransfer_;
    }

    OutboundTransfer* outboundTransfer() {
        return &outboundTransfer_;
    }

    virtual void initialize(const ChannelPipeline& pipeline) {
        ChannelHandlerContext::initialize(pipeline);
        inboundContainer_.setEventLoop(eventLoop());
        outboundContainer_.setEventLoop(eventLoop());
    }

private:
    virtual boost::any getInboundMessageContainer() {
        return boost::any(&inboundContainer_);
    }

    virtual boost::any getOutboundMessageContainer() {
        return boost::any(&outboundContainer_);
    }

    virtual void onPipelineChanged() {
        ChannelHandlerContext::onPipelineChanged();

        outboundTransfer_.resetNextContainer();
        inboundTransfer_.resetNextContainer();
    }

private:
    StoredHandlerPtr handler_;

    InboundInContainer inboundContainer_;
    OutboundInContainer outboundContainer_;

    InboundTransfer inboundTransfer_;
    OutboundTransfer outboundTransfer_;
};

template<typename H>
class ChannelMessageHandlerContext<H,
    VoidMessage,
    VoidMessage,
    VoidMessage,
    VoidMessage,
    VoidMessageContainer,
    VoidMessageContainer,
    VoidMessageContainer,
        VoidMessageContainer> : public ChannelHandlerContext {
public:
    typedef ChannelMessageHandlerContext<H,
            VoidMessage,
            VoidMessage,
            VoidMessage,
            VoidMessage,
            VoidMessageContainer,
            VoidMessageContainer,
            VoidMessageContainer,
            VoidMessageContainer> Context;

    typedef typename ChannelHandlerWrapper<H>::Handler Handler;
    typedef typename ChannelHandlerWrapper<H>::HandlerPtr HandlerPtr;
    typedef typename ChannelHandlerWrapper<H>::StoredHandlerPtr StoredHandlerPtr;

    typedef boost::function<void (HandlerPtr const&, Context&)> RegisterCallback;

    typedef ChannelMessageTransfer<VoidMessage, VoidMessageContainer, TRANSFER_INBOUND> InboundTransfer;
    typedef ChannelMessageTransfer<VoidMessage, VoidMessageContainer, TRANSFER_OUTBOUND> OutboundTransfer;

public:
    ChannelMessageHandlerContext(const std::string& name,
                                 const HandlerPtr& handler)
        : ChannelHandlerContext(name),
          handler_(handler) {
        if (handler) {
            handler->registerTo(*this);
        }
    }

    ChannelMessageHandlerContext(const std::string& name,
                                 const HandlerPtr& handler,
                                 const RegisterCallback& registerCallback)
        : ChannelHandlerContext(name),
          handler_(handler) {
        if (handler && registerCallback) {
            registerCallback(handler, *this);
        }
    }

    ChannelMessageHandlerContext(const std::string& name,
                                 const EventLoopPtr& eventLoop,
                                 const HandlerPtr& handler)
        : ChannelHandlerContext(name, eventLoop),
          handler_(handler) {
        if (handler) {
            handler->registerTo(*this);
        }
    }

    ChannelMessageHandlerContext(const std::string& name,
                                 const HandlerPtr& handler,
                                 const EventLoopPtr& eventLoop,
                                 const RegisterCallback& registerCallback)
        : ChannelHandlerContext(name, eventLoop),
          handler_(handler) {
        if (handler && registerCallback) {
            registerCallback(handler, *this);
        }
    }

    VoidMessageContainer* inboundContainer() { return NULL; }
    VoidMessageContainer* outboundContainer() { return NULL; }

    InboundTransfer* inboundTransfer() { return NULL; }
    OutboundTransfer* outboundTransfer() { return NULL; }

private:
    virtual boost::any getInboundMessageContainer() {
        return boost::any();
    }

    virtual boost::any getOutboundMessageContainer() {
        return boost::any();
    }

private:
    StoredHandlerPtr handler_;
};

}
}

#endif //#if !defined(CETTY_CHANNEL_CHANNELMESSAGEHANDLERCONTEXT_H)

// Local Variables:
// mode: c++
// End:
