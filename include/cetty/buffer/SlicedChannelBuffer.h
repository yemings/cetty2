#if !defined(CETTY_BUFFER_SLICEDCHANNELBUFFER_H)
#define CETTY_BUFFER_SLICEDCHANNELBUFFER_H

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

#include <cetty/buffer/ChannelBuffer.h>
#include <cetty/buffer/WrappedChannelBuffer.h>

namespace cetty {
namespace buffer {

/**
 * A derived buffer which exposes its parent's sub-region only.  It is
 * recommended to use {@link ChannelBuffer#slice()} and
 * {@link ChannelBuffer#slice(int, int)} instead of calling the constructor
 * explicitly.
 *
 *
 * @author <a href="http://gleamynode.net/">Trustin Lee</a>
 *
 * @author <a href="mailto:frankee.zhou@gmail.com">Frankee Zhou</a>
 *
 */

class SlicedChannelBuffer : public WrappedChannelBuffer {
public:
    SlicedChannelBuffer(const ChannelBufferPtr& buffer,
                        int index,
                        int length);

    virtual ~SlicedChannelBuffer() {}

    virtual const ChannelBufferPtr& unwrap();

    virtual int capacity() const;

    virtual void capacity(int newCapacity);

    virtual const char* readableBytes(int* length);
    virtual char* writableBytes(int* length);
    virtual char* aheadWritableBytes(int* length);

    virtual int8_t  getByte(int index) const;
    virtual int16_t getShort(int index) const;
    virtual int32_t getInt(int index) const;
    virtual int64_t getLong(int index) const;

    virtual int getBytes(int index, char* dst, int length) const;
    virtual int getBytes(int index, OutputStream* out, int length) const;
    virtual int getBytes(int index, const ChannelBufferPtr& dst, int dstIndex, int length) const;

    virtual int setByte(int index, int value);
    virtual int setShort(int index, int value);
    virtual int setInt(int index, int value);
    virtual int setLong(int index, int64_t value);

    virtual int setBytes(int index, const char* src, int length);
    virtual int setBytes(int index, InputStream* in, int length);
    virtual int setBytes(int index, const ConstChannelBufferPtr& src, int srcIndex, int length);

    virtual ChannelBufferPtr slice(int index, int length);
    virtual int slice(int index, int length, GatheringBuffer* gathering);

    virtual ChannelBufferPtr copy(int index, int length) const;

private:
    void checkIndex(int index) const;
    void checkIndex(int startIndex, int length) const;

private:
    int length;
    int adjustment;

    ChannelBufferPtr channelBuffer;
};

}
}

#endif //#if !defined(CETTY_BUFFER_SLICEDCHANNELBUFFER_H)

// Local Variables:
// mode: c++
// End:
