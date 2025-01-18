//
// Created by lhy on 2024/9/20.
//

#ifndef OWN_CPPTASK_H
#define OWN_CPPTASK_H

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

#ifdef __cplusplus
}
#endif


void DJITask();
void LKTask();
void LedTask();
void ErrorTask();
void DetectTask();
void ChassisTask();
void RemoteCtrlTask();
void IMUTask();
void StartTask();
void PCTask();
#endif //OWN_CPPTASK_H
