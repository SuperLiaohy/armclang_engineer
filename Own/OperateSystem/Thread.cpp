//
// Created by lhy on 2024/9/17.
//

#include "Thread.h"
#include "Thread/CppTask.h"
#include "ThreadConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

EventGroupHandle_t osEventGroup;

UBaseType_t StartHeapCnt = 0;
UBaseType_t LedHeapCnt = 0;
UBaseType_t CANHeapCnt = 0;
UBaseType_t ErrorHeapCnt = 0;
UBaseType_t DetectHeapCnt = 0;
UBaseType_t ChassisHeapCnt = 0;
UBaseType_t RemoteCtrlHeapCnt = 0;
UBaseType_t IMUHeapCnt = 0;
#ifdef __cplusplus
}
#endif

void StartDefaultTask(void const *argument) {
    UNUSED(argument);
    StartTask();
}

void OS_LedTask(void const *argument) {
    UNUSED(argument);
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
		

    LedTask();
}

void OS_DJITask(void const *argument) {
    UNUSED(argument);
//    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_START_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    xEventGroupWaitBits(osEventGroup, CAN_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, IMU_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, START_END_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
		

    DJITask();
}

void OS_ErrorTask(void const *argument) {
    UNUSED(argument);


    ErrorTask();
}

void OS_DetectTask(void const *argument) {
    UNUSED(argument);

//    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_START_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    xEventGroupWaitBits(osEventGroup, CAN_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, IMU_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, START_END_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    DetectTask();
}

void OS_ChassisTask(void const *argument) {
    UNUSED(argument);

//    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_START_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, CAN_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, IMU_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, START_END_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    ChassisTask();
}

void OS_RemoteCtrlTask(void const *argument) {
    UNUSED(argument);
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_START_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);


    RemoteCtrlTask();
}

void OS_IMUTask(void const *argument) {
    UNUSED(argument);


    IMUTask();
}


void OS_LKTask(void const *argument) {
    UNUSED(argument);

//    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_START_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    xEventGroupWaitBits(osEventGroup, CAN_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, IMU_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, START_END_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    LKTask();
}

void OS_PCTask(void const *argument) {
    UNUSED(argument);

//    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_START_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    xEventGroupWaitBits(osEventGroup, CAN_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, IMU_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(osEventGroup, START_END_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    PCTask();
}

void OS_ImageTransTask(void const *argument) {
    UNUSED(argument);

    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    ImageTransTask();
}

void OS_JudgeTask(void const *argument) {
    UNUSED(argument);

    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

//    xEventGroupWaitBits(osEventGroup, CAN_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
//    xEventGroupWaitBits(osEventGroup, IMU_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
//    xEventGroupWaitBits(osEventGroup, START_END_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    JudgeTask();
}
