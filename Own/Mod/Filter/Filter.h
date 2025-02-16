//
// Created by lhy on 2024/9/15.
//

#ifndef NEW_PRJ_FILTER_H
#define NEW_PRJ_FILTER_H
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


#endif //NEW_PRJ_FILTER_H
