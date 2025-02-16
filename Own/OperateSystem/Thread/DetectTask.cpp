//
// Created by liaohy on 24-11-15.
//
#include "CppTask.h"

#include "Detect/Detect.h"


void DetectTask() {
    while (1) {
        Detect::detectManager.JudgeLost();
        DetectHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelay(10);
    }
}
