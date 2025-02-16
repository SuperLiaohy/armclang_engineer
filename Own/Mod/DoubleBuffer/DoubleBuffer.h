//
// Created by liaohy on 8/26/24.
//

#ifndef NONE_PRJ_DOUBLEBUFFER_H
#define NONE_PRJ_DOUBLEBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#ifdef __cplusplus
}
#endif


class DoubleBuffer {
private:
    struct Buffer {
        uint8_t *data;
        uint16_t size;
        uint16_t count;
    } buffers[2]{};

    uint8_t activeBuffer;
public:
    explicit DoubleBuffer(uint16_t bufferSize);

    ~DoubleBuffer();

    Buffer *switch_buffer();

    Buffer *next(int32_t size);

    uint8_t *read(uint16_t &size);

    void clear();

    uint16_t correct_count(uint16_t size);
};


#endif //NONE_PRJ_DOUBLEBUFFER_H
