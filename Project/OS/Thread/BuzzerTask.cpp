//
// Created by Administrator on 2025/3/29.
//



#include "Buzzer/Buzzer.hpp"

extern "C" void OS_BuzzerTask(void const *argument) {
    while (1) {
        // if (!buzzer.StartMusic())
            osDelay(2000);
    }
}