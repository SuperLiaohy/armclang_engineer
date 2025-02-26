//
// Created by liaohy on 24-11-15.
//
#include "CppTask.hpp"

#include "Detect/Detect.hpp"


void DetectTask() {
    while (1) {
        Detect::detectManager.JudgeLost();
        DetectHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelay(10);
    }
}
