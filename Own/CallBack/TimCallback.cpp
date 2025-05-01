//
// Created by Administrator on 25-5-1.
//
#include <tim.h>
#include <Detect/Detect.hpp>
extern "C" void UserTimCallback(TIM_HandleTypeDef* htim) {
    if (htim == &htim6) {
        DetectManagerInstance<TimDetect>().JudgeLost();

    }
}