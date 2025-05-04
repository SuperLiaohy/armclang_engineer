//
// Created by lhy on 2024/9/15.
//
#pragma once


#include "Buzzer/Buzzer.hpp"

namespace remote_ctrl_dep {
    [[gnu::always_inline]] inline float addSpeed(float rc_ch, float max_speed) {
        return rc_ch * max_speed / 660.f;
    }
    enum status {
        LOST,
        KEYBOARD,
        NORMAL
    };
    enum class lever {
        upper = 1,
        middle = 3,
        lower = 2,
    };

    struct RC_TypeDef {
        /* rocker channel information */
        int16_t ch1;
        int16_t ch2;
        int16_t ch3;
        int16_t ch4;
        /* left and right lever information */
        uint8_t left;
        uint8_t right;
        /* mouse movement and button information */
        int16_t wheel;
    };

}


