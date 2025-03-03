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

extern float left_angle;
extern float right_angle;


MicroTime microTime;

void DJITask() {
//    osDelay(1000);
    while (1) {
        auto now = osKernelSysTick();
        microTime.start();
        roboArm.load_diff_target(interact);
        left_angle = roboArm.diff.left.feed_back.totalPosition;
        right_angle = roboArm.diff.right.feed_back.totalPosition;

        roboArm.diff.left.motor.doublePid.update(left_angle, roboArm.target.joint5.angle, roboArm.diff.left.feed_back.data.speed);
        roboArm.diff.right.motor.doublePid.update(right_angle, roboArm.target.joint6.angle, roboArm.diff.right.feed_back.data.speed);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        canPlus1.write(roboArm.diff.left.motor.doublePid.output, roboArm.diff.right.motor.doublePid.output, 0, 0);
        canPlus1.send(Motor<M2006>::foc.TX_LOW_ID);
        xSemaphoreGive(CAN1MutexHandle);

        microTime.end();
        CANHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelayUntil(&now, 1);

    }
}