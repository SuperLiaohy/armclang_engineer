//
// Created by Administrator on 24-10-3.
//
#include "ThreadConfig.h"

#include "OneStepGet/OneStepGet.hpp"
#include "WDG/SuperIWDG.hpp"

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    UNUSED(RxFifo0ITs);
    UNUSED(hfdcan);

}

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
    UNUSED(ErrorStatusITs);
    UNUSED(hfdcan);

}