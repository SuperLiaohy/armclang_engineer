//
// Created by Administrator on 2025/4/4.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "RoboArm/RoboArm.hpp"

void ArmTask() {
    uint32_t cnt = 0;
    while (true) {
        auto now = osKernelSysTick();
        ++cnt;

        roboArm.update_relative_pos();
        roboArm.load_target(interact.joint);

        roboArm.diff.left.set_position(roboArm.target.joint5.angle);
        roboArm.diff.right.set_position(roboArm.target.joint6.angle);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        canPlus1.transmit(M2006Diff::foc.TX_LOW_ID, roboArm.diff.left.output(), roboArm.diff.right.output(), 0, 0);
        xSemaphoreGive(CAN1MutexHandle);

        roboArm.diff.write_fram();

        if (cnt % 10 == 0) {
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint3.set_position(roboArm.target.joint3.angle, 180);
            xSemaphoreGive(CAN1MutexHandle);

            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint4.set_position_near(roboArm.target.joint4.angle, 720);
            xSemaphoreGive(CAN1MutexHandle);
        }
        if ((cnt + 1) % 10 == 0) {
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint2.internal.set_position(roboArm.target.joint2.internal.angle, 180);
            roboArm.joint2.external.set_position(roboArm.target.joint2.external.angle, 180);
            xSemaphoreGive(CAN1MutexHandle);
        }
        if ((cnt + 2)%10 == 0) {
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint1.set_position(roboArm.target.joint1.angle, 480);
            xSemaphoreGive(CAN1MutexHandle);

            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint4.read_totalposition();
            xSemaphoreGive(CAN1MutexHandle);
        }
        if ((cnt+3) % 10 == 0) {
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint3.read_totalposition();
            xSemaphoreGive(CAN1MutexHandle);

            // 相对旋转角度只需要一个电机就可以确定，所以只需要一个电机的反馈数据，这里选择外侧电机
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint2.external.read_totalposition();
            xSemaphoreGive(CAN1MutexHandle);

            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint1.read_totalposition();
            xSemaphoreGive(CAN1MutexHandle);
        }
        CANHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelayUntil(&now, 1);
    }
}