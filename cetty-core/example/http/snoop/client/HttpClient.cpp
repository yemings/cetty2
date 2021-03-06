/*
 * Copyright 2009 Red Hat, Inc.
 *
 * Red Hat licenses this file to you under the Apache License, version 2.0
 * (the "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
/*
 * Copyright (c) 2010-2011 frankee zhou (frankee.zhou at gmail dot com)
 * Distributed under under the Apache License, version 2.0 (the "License").
 */

#include <cetty/bootstrap/ClientBootstrap.h>
#include <cetty/channel/Channel.h>
#include <cetty/channel/InetAddress.h>
#include <cetty/channel/ChannelPipelines.h>
#include <cetty/channel/asio/AsioClientSocketChannelFactory.h>

#include <cetty/handler/codec/http/HttpHeaders.h>
#include <cetty/handler/codec/http/HttpRequest.h>
#include <cetty/handler/codec/http/HttpClientCodec.h>

#include <cetty/util/URI.h>

#include "HttpResponseHandler.h"

using namespace cetty::bootstrap;
using namespace cetty::util;
using namespace cetty::channel;
using namespace cetty::channel::socket::asio;
using namespace cetty::handler::codec::http;


ChannelPipelinePtr getPipeline(bool ssl) {
    // Create a default pipeline implementation.
    ChannelPipelinePtr pipeline = ChannelPipelines::pipeline();

    // Enable HTTPS if necessary.
    if (ssl) {
        //             SSLEngine engine =
        //                 SecureChatSslContextFactory.getClientContext().createSSLEngine();
        //             engine.setUseClientMode(true);
        // 
        //             pipeline.addLast("ssl", new SslHandler(engine));
    }

    pipeline->addLast("codec", new HttpClientCodec);

    // Remove the following line if you don't want automatic content decompression.
    //pipeline->addLast("inflater", new HttpContentDecompressor());

    // Uncomment the following line if you don't want to handle HttpChunks.
    //pipeline.addLast("aggregator", new HttpChunkAggregator(1048576));

    pipeline->addLast("handler", new HttpResponseHandler());
    return pipeline;
}

/**
 * A simple HTTP client that prints out the content of the HTTP response to
 * {@link System#out} to test {@link HttpServer}.
 *
 * @author <a href="http://www.jboss.org/netty/">The Netty Project</a>
 * @author Andy Taylor (andy.taylor@jboss.org)
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 *
 * @version $Rev: 2226 $, $Date: 2010-03-31 11:26:51 +0900 (Wed, 31 Mar 2010) $
 */
int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("Usage: SnoopClient <URL>");
        return -1;
    }

    URI uri(argv[1]);
    std::string scheme = uri.getScheme().empty() ? "http" : uri.getScheme();
    std::string host = uri.getHost().empty() ? "localhost" : uri.getHost();
    int port = uri.getPort();

    if (port == -1) {
        if (scheme == "http") {
            port = 80;
        }
        else if (scheme == "https") {
            port = 443;
        }
    }

    if ((scheme != "http") && (scheme != "https")) {
        printf("Only HTTP(S) is supported.");
        return -1;
    }

    bool ssl = scheme == "https";

    // Configure the client.
    ClientBootstrap bootstrap(
        ChannelFactoryPtr(new AsioClientSocketChannelFactory(1)));

    // Set up the event pipeline factory.
    bootstrap.setPipeline(getPipeline(ssl));

    // Start the connection attempt.
    ChannelFuturePtr future = bootstrap.connect(InetAddress(host, port));

    // Wait until the connection attempt succeeds or fails.
    ChannelPtr channel = future->awaitUninterruptibly()->channel();

    if (!future->isSuccess()) {
        bootstrap.shutdown();
        return -1;
    }

    // Prepare the HTTP request.
    HttpRequestPtr request =
        HttpRequestPtr(new HttpRequest(HttpVersion::HTTP_1_1,
                       HttpMethod::GET,
                       uri.toString()));

    request->setHeader(HttpHeaders::Names::HOST, host);
    request->setHeader(HttpHeaders::Names::CONNECTION, HttpHeaders::Values::CLOSE);
    request->setHeader(HttpHeaders::Names::ACCEPT_ENCODING, HttpHeaders::Values::GZIP);

    // Set some example cookies.
    //CookieEncoder httpCookieEncoder = new CookieEncoder(false);
    //httpCookieEncoder.addCookie("my-cookie", "foo");
    //httpCookieEncoder.addCookie("another-cookie", "bar");
    //request->setHeader(HttpHeaders::Names::COOKIE, httpCookieEncoder.encode());

    // Send the HTTP request.
    channel->write(request);

    // Wait for the server to close the connection.
    channel->closeFuture()->awaitUninterruptibly();

    // Shut down executor threads to exit.
    bootstrap.shutdown();

    return 0;
};
