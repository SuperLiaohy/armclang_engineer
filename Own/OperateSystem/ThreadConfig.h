//
// Created by lhy on 2024/9/20.
//

#ifndef OWN_THREADCONFIG_H
#define OWN_THREADCONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
#include "cmsis_os.h"
extern EventGroupHandle_t osEventGroup;

#define ENABLE_CHASSIS 1
#define ENABLE_IMU 1

#define ENABLE_IMU_CALIBRATION 1
#define ENABLE_CAN_SEND 1

#define IMU_EVENT (1<<0)
#define CAN_SEND_EVENT (1<<1)
#define CAN_RECEIVE_EVENT (1<<2)
#define REMOTE_CONTROL_START_EVENT (1<<3)
#define REMOTE_CONTROL_RECEIVE_EVENT (1<<4)
#define START_END_EVENT (1<<5)
#define LK_RELETIVE_GET (1<<6)


#ifdef __cplusplus
};
#endif
#endif //OWN_THREADCONFIG_H
