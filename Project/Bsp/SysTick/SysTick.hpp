//
// Created by Administrator on 2025/3/23.
//

#pragma once

#include "stm32h7xx_hal.h"

class Sys {
public:
    static Sys& GetInstance() {
        static Sys instance;
        return instance;
    }

    static inline uint32_t get_tick() {
        return SysTick->VAL;
    }
private:
    Sys() = default;

};
