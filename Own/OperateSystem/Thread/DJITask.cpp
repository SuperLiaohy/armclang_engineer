//
// Created by liaohy on 24-11-15.
//
#include "CppTask.hpp"
#include "Vofa/vofa.hpp"
#include "Motor/lkMotor.hpp"
#include "Motor/GM6020.hpp"
#include "Uart/SuperUart.hpp"
#include "CDC/SuperCDC.hpp"
#include "Imu/Imu.hpp"
#include "RoboArm/RoboArm.hpp"
#include "Interact/Interact.hpp"
#include "MicroTime/MicroTime.hpp"



void DJITask() {
//    osDelay(1000);
    while (1) {
        auto now = osKernelSysTick();
        // roboArm.load_diff_target(interact.joint);
        //
        // roboArm.diff.left.motor.set_position(roboArm.target.joint5.angle);
        // roboArm.diff.right.motor.set_position(roboArm.target.joint6.angle);
        //
        // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        // canPlus1.transmit(M2006::foc.TX_LOW_ID, roboArm.diff.left.motor.output(), roboArm.diff.right.motor.output(), 0, 0);
        // xSemaphoreGive(CAN1MutexHandle);
        //
        // CANHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelayUntil(&now, 1);

    }
}