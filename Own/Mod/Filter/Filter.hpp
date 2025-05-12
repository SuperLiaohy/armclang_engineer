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

#include "RingQueue/RingQueue.hpp"

template<uint32_t n>
class Filter {
public:
    explicit Filter() : queue(), sum(0) {};

    float update(float value);

private:
    RingQueue<float, n> queue;
    float sum;
};


