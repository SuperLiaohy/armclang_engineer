//
// Created by liaohy on 24-11-15.
//
#include "CppTask.hpp"
#include "Detect/Detect.hpp"
#include <Count/Count.hpp>
#include <WDG/SuperIWDG.hpp>

void DebugTask() {
    uint32_t time    = 0;
    CountManager& it = CountManagerInstance();
    while (1) {
        auto now = xTaskGetTickCount();
        ++time;
        DetectManagerInstance().JudgeLost();
        DetectHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        if (time > 99) {
            it.mark();
            time = 0;
        }
        SuperIWDG::GotInstance().give();
        osDelayUntil(&now, 10);
    }
}
