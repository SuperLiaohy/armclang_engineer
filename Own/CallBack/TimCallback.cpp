//
// Created by Administrator on 25-5-1.
//
#include <tim.h>
#include <Detect/Detect.hpp>
#include <Count/Count.hpp>

Count count;
extern "C" void UserTimCallback(TIM_HandleTypeDef* htim) {
    if (htim == &htim6) {
        TimDetectManagerInstance().JudgeLost();
        ++count;
    }
}