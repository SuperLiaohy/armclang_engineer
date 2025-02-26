//
// Created by liaohy on 24-11-15.
//
#include "CppTask.hpp"

#include "RemoteControl/RemoteControl.hpp"
#include "RGBLED/RGBLED.hpp"

void LedTask() {
    while (1) {

        Led.update();
        LedHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelay(1);
    }
}