//
// Created by liaohy on 24-11-15.
//

#include "RGBLED/RGBLED.hpp"

extern "C" void OS_LedTask(void const*argument) {
    while (1) {
        Led.update();
        osDelay(1);
    }
}
