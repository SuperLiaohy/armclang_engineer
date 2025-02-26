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

#include "RoundQueue.h"


class Filter {
public:
    explicit Filter(const uint32_t size) : queue(RoundQueue(static_cast<int8_t>(size))), sum(0) {};

    float update(float value);

private:
    RoundQueue queue;
    float sum;
};


