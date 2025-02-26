//
// Created by Administrator on 24-10-3.
//
#include "ThreadConfig.h"
#include "CAN/SuperCan.hpp"
#include "Motor/GM6020.hpp"
#include "Motor/lkMotor.hpp"
#include "Motor/dmMotor.hpp"
#include "Motor/M2006.hpp"
#include "RoboArm/RoboArm.hpp"
#include "Chassis/Chassis.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "cmsis_os.h"

#ifdef __cplusplus
}
#endif


extern float yaw;

float a =0;
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    UNUSED(RxFifo0ITs);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
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
            xEventGroupSetBitsFromISR(osEventGroup, DIFF_LEFT_RECEIVE_EVENT, &xHigherPriorityTaskWoken);
        }
        else if (canPlus1.rx_header.Identifier == Motor<M2006>::foc.RX_ID + roboArm.diff.right.feed_back.id) {
            roboArm.diff.right.readData(canPlus1.rx_data);
            xEventGroupSetBitsFromISR(osEventGroup, DIFF_RIGHT_RECEIVE_EVENT, &xHigherPriorityTaskWoken);
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    } else if (hfdcan == chassis.can->hcan){
        chassis.can->receive();
        chassis.UpdateMotor();
    }
}
