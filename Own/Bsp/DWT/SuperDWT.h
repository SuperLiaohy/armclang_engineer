//
// Created by Administrator on 25-2-15.
//
#pragma once

#include "stm32h7xx_hal.h"

class SuperDWT {
public:
    static inline SuperDWT& GetInstance() {
        static SuperDWT instance;
        return instance;
    }

private:
    SuperDWT() {
        if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
            CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        }
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }





};


