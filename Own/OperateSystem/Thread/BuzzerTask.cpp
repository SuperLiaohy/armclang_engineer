//
// Created by Administrator on 2025/3/29.
//



#include "Buzzer/Buzzer.hpp"
#include "CppTask.hpp"

void BuzzerTask() {
    while (1) {
        if (buzzer.StartMusic()) {
           osDelay(10);
        }
    }
}