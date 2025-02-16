//
// Created by liaohy on 8/26/24.
//

#include "DoubleBuffer.h"
#include "Heap/CustomHeap.h"

DoubleBuffer::DoubleBuffer(const uint16_t bufferSize) {
    for (auto &[data, size, count]: buffers) {
        data = static_cast<uint8_t *>(D1Heap.malloc(bufferSize));
        size = bufferSize;
        count = 0;
    }
    activeBuffer = 0;
}


DoubleBuffer::~DoubleBuffer() {
    for (auto &buffer: buffers) {
        D1Heap.free(buffer.data);
    }
}

DoubleBuffer::Buffer *DoubleBuffer::next(int32_t size) {
    auto buf = &buffers[activeBuffer];
    buf->count = size;
    return buf;
}


uint8_t *DoubleBuffer::read(uint16_t &size) {
    Buffer &buf = buffers[1 - activeBuffer];
    size = buf.count;
    clear();
    return buf.data;
}

void DoubleBuffer::clear() {
    Buffer &buf = buffers[1 - activeBuffer];
    buf.count = 0;
}

uint16_t DoubleBuffer::correct_count(uint16_t size) {
    buffers[1 - activeBuffer].count = size;
    return size;
}

DoubleBuffer::Buffer *DoubleBuffer::switch_buffer() {
    activeBuffer = 1 - activeBuffer;
    return &buffers[1 - activeBuffer];
}




