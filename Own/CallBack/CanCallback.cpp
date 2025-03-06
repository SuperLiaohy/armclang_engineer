//
// Created by Administrator on 24-10-3.
//
#include "ThreadConfig.h"
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
        roboArm.joint4.motor.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint3.motor.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint2.external.motor.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint2.internal.motor.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint1.motor.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);


        // if (canPlus1.rx_header.Identifier == LKMotor::foc.RX_ID + roboArm.joint4.feed_back.id) {
            // roboArm.joint4.get_feedback(canPlus1.rx_data);
        // }
        // else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint3.feed_back.id) {
            // roboArm.joint3.get_feedback(canPlus1.rx_data);
        // }
        // else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint2.internal.feed_back.id) {
            // roboArm.joint2.internal.get_feedback(canPlus1.rx_data);
        // }
        // else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint2.external.feed_back.id) {
            // roboArm.joint2.external.get_feedback(canPlus1.rx_data);
        // }
        // else if (canPlus1.rx_header.Identifier == Motor<lkMotor>::foc.RX_ID + roboArm.joint1.feed_back.id) {
            // roboArm.joint1.get_feedback(canPlus1.rx_data);
        // }
        // else if (canPlus1.rx_header.Identifier == M2006::foc.RX_ID + roboArm.diff.left.feed_back.id) {
            roboArm.diff.left.motor.get_feedback(canPlus1.rx_header.Identifier - M2006::foc.RX_ID, canPlus1.rx_data);
            xEventGroupSetBitsFromISR(osEventGroup, DIFF_LEFT_RECEIVE_EVENT, &xHigherPriorityTaskWoken);
        // }
        // else if (canPlus1.rx_header.Identifier == M2006::foc.RX_ID + roboArm.diff.right.feed_back.id) {
            roboArm.diff.right.motor.get_feedback(canPlus1.rx_header.Identifier - M2006::foc.RX_ID,canPlus1.rx_data);
            xEventGroupSetBitsFromISR(osEventGroup, DIFF_RIGHT_RECEIVE_EVENT, &xHigherPriorityTaskWoken);
        // }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    } else if (hfdcan == chassis.can->hcan){
        chassis.can->receive();
        chassis.UpdateMotor();
    }
}
