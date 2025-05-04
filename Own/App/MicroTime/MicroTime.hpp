//
// Created by Administrator on 25-1-15.
//
#pragma once

#include "SysTick/SysTick.hpp"

class MicroTime {
public:

    MicroTime() = default;
    [[gnu::always_inline]] inline void start();
    [[gnu::always_inline]] inline uint32_t end();
    [[gnu::always_inline]] inline uint32_t get_time();
    [[gnu::always_inline]] inline static void us_delay(uint32_t us);
private:
    uint32_t time;
    uint32_t start_time;
    uint32_t end_time;

    uint32_t count;
};

[[gnu::always_inline]] inline void MicroTime::start() {
    count      = osKernelSysTick();
    start_time = SysTick->VAL;
}

[[gnu::always_inline]] inline uint32_t MicroTime::end() {
    end_time = SysTick->VAL;
    count = osKernelSysTick() - count;
    if (start_time >= end_time) {
        time = (start_time - end_time) * 1000000 / SystemCoreClock + count * 1000;
    } else {
        // Handle SysTick counter overflow
        time = ((SysTick->LOAD - end_time + start_time) * 1000000 / SystemCoreClock) + count * 1000;
    }
    return time;
}

[[gnu::always_inline]] inline uint32_t MicroTime::get_time() {
    return time;
}

[[gnu::always_inline]] inline void MicroTime::us_delay(uint32_t us) {
    uint32_t start_time=SysTick->VAL, end_time=0, count=osKernelSysTick();
    do {
        end_time = SysTick->VAL;
    } while((end_time - start_time) * 1000000 / SystemCoreClock + (osKernelSysTick()-count) * 1000 < us);
}
