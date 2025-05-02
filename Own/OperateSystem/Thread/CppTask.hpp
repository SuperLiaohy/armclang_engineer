//
// Created by lhy on 2024/9/20.
//
#pragma once


#ifdef __cplusplus
extern "C" {
#endif
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

extern UBaseType_t StartHeapCnt;
extern UBaseType_t LedHeapCnt;
extern UBaseType_t CANHeapCnt;
extern UBaseType_t ErrorHeapCnt;
extern UBaseType_t DetectHeapCnt;
extern UBaseType_t ChassisHeapCnt;
extern UBaseType_t RemoteCtrlHeapCnt;
extern UBaseType_t IMUHeapCnt;
extern osMutexId CAN1MutexHandle;
extern osThreadId ARM_INIT_TASKHandle;
#ifdef __cplusplus
}
#endif

void LedTask();
void ErrorTask();
void DebugTask();
void ChassisTask();
void RemoteCtrlTask();
void IMUTask();
void StartTask();
void PCTask();
void ImageTransTask();
void JudgeTask();
void KbTask();
void SubBoardTask();
void ArmInitTask();
void BuzzerTask();
void ArmTask();
void OneStepGetTask();
