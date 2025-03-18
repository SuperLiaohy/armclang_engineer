//
// Created by lhy on 2024/9/15.
//
#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#ifdef __cplusplus
}
#endif

template<typename T, uint32_t n>
class RoundQueue {
public:
    explicit RoundQueue() : head(0), tail(0), length(0) {}

    bool pop(T& value);

    void push(const T&value);

    void clear() {
        head = 0;
        tail = 0;
        length = 0;
    }

    [[nodiscard]] int32_t len() const {
        return (tail - head + n) % n;
    }

private:
    float buffer[n]{};

    int8_t head;
    int8_t tail;
    int8_t length;

};


