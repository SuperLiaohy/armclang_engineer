//
// Created by Administrator on 24-10-3.
//
#include "ThreadConfig.h"
#include "RoboArm/RoboArm.hpp"
#include "Chassis/Chassis.hpp"
#include "OneStepGet/OneStepGet.hpp"
#include "WDG/SuperIWDG.hpp"

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    UNUSED(RxFifo0ITs);
    SuperIWDG::GotInstance().give();
    if (hfdcan == canPlus1.hcan) {
        canPlus1.receive();
        roboArm.joint4.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint3.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint2.external.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint2.internal.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
//        roboArm.joint1.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint5.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
        roboArm.joint6.get_feedback(canPlus1.rx_header.Identifier - LKMotor::foc.RX_ID, canPlus1.rx_data);
    } else if (hfdcan == chassis.can->hcan) {
        chassis.can->receive();
        chassis.UpdateMotor();
    } else if (hfdcan == canPlus3.hcan) {
        canPlus3.receive();
        one_step_gets.Xleft.Motor.get_feedback(canPlus3.rx_header.Identifier - M2006::foc.RX_ID, canPlus3.rx_data);
        one_step_gets.Yleft.Motor.get_feedback(canPlus3.rx_header.Identifier - M3508::foc.RX_ID, canPlus3.rx_data);
        one_step_gets.Xright.Motor.get_feedback(canPlus3.rx_header.Identifier - M2006::foc.RX_ID, canPlus3.rx_data);
        one_step_gets.Yright.Motor.get_feedback(canPlus3.rx_header.Identifier - M3508::foc.RX_ID, canPlus3.rx_data);
        one_step_gets.rotate_move.Motor.get_feedback(canPlus3.rx_header.Identifier - M2006::foc.RX_ID,canPlus3.rx_data);
//        if (canPlus3.rx_header.Identifier < 100) {
        one_step_gets.rotate.get_feedback(canPlus3.rx_header.Identifier, canPlus3.rx_data);
//        }
    }
}

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs) {
    if (hfdcan == canPlus1.hcan) {
        ++canPlus1.err_cnt;
    } else if (hfdcan == canPlus2.hcan) {
        ++canPlus2.err_cnt;
    } else if (hfdcan == canPlus3.hcan) {
        ++canPlus3.err_cnt;
    }
}