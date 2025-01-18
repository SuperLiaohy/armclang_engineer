//
// Created by liaohy on 24-11-15.
//
#include "CppTask.h"

#include "RemoteControl/RemoteControl.h"
#include "RGBLED/RGBLED.h"

void LedTask() {
    while (1) {

        Led.update();
        LedHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelay(1);
    }
}