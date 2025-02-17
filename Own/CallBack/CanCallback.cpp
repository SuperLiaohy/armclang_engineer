//
// Created by Administrator on 24-10-3.
//
#include "ThreadConfig.h"
#include "CAN/SuperCan.h"
#include "Motor/GM6020.h"
#include "Motor/lkMotor.h"
#include "Motor/dmMotor.h"
#include "Motor/M2006.h"
#include "RoboArm/RoboArm.h"
#include "Chassis/Chassis.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "cmsis_os.h"

#ifdef __cplusplus
}
#endif


extern float yaw;
float relative_position = 0;
float GM6020pos = 0;
float lkMotorSpeed = 0;
float dmMotorSpeed = 0;
float dmMotorPos = 0;
float a =0;
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    UNUSED(RxFifo0ITs);
    if (hfdcan == canPlus1.hcan) {
			++a;
        canPlus1.receive();
        if (canPlus1.rx_header.Identifier == Motor<lkMotorBoard>::foc.RX_ID + test_motor.feed_back.id-100) {
            test_motor.readData(canPlus1.rx_data);
        }
        else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint4.feed_back.id) {
            roboArm.joint4.readData(canPlus1.rx_data);
        }
        else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint3.feed_back.id) {
            roboArm.joint3.readData(canPlus1.rx_data);
        }
        else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint2.internal.feed_back.id) {
            roboArm.joint2.internal.readData(canPlus1.rx_data);
        }
        else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint2.external.feed_back.id) {
            roboArm.joint2.external.readData(canPlus1.rx_data);
        }
        else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint1.feed_back.id) {
            roboArm.joint1.readData(canPlus1.rx_data);
        }
        else if (canPlus1.rx_header.Identifier == Motor<M2006>::foc.RX_ID + roboArm.diff.left.feed_back.id) {
            roboArm.diff.left.readData(canPlus1.rx_data);
        }
        else if (canPlus1.rx_header.Identifier == Motor<M2006>::foc.RX_ID + roboArm.diff.right.feed_back.id) {
            roboArm.diff.right.readData(canPlus1.rx_data);
        }
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xEventGroupSetBitsFromISR(osEventGroup, CAN_RECEIVE_EVENT, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    } else if (hfdcan == chassis.can->hcan){
        chassis.can->receive();
        chassis.UpdateMotor();
    }
}
