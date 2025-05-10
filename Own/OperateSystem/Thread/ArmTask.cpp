//
// Created by Administrator on 2025/4/4.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
extern LKPidMotor lk4005;
volatile float lk_target = 300;
void ArmTask() {
    uint32_t cnt      = 0;
    auto can_transmit = [](LKPidMotor& motor) {
        auto output     = motor.output();
        uint8_t data[8] = {0xa1, 0, 0, 0, *(uint8_t*)(&output), *((uint8_t*)(&output) + 1), 0, 0};
        canPlus1.transmit_pdata(motor.rx_id + LKSingleMotor::foc.TX_LOW_ID, data);
    };
    // while (true) {
    //     auto now = osKernelSysTick();
    while (true) {
        ++cnt;
        lk4005.set_position(lk_target);
        lk4005.torqueControl();
        // can_transmit(lk4005);
        osDelay(2);
    }
    //     roboArm.update_relative_pos();
    //     roboArm.load_target(interact.joint);
    //
    //     roboArm.diff.left.set_position(roboArm.target.joint5.angle);
    //     roboArm.diff.right.set_position(roboArm.target.joint6.angle);
    //
    //     xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //     canPlus1.transmit(M2006Diff::foc.TX_LOW_ID, roboArm.diff.left.speed_output(), roboArm.diff.right.speed_output(), 0, 0);
    //     xSemaphoreGive(CAN1MutexHandle);
    //
    //     roboArm.diff.write_fram();
    //
    //     if (cnt % 10 == 0) {
    //         xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //         roboArm.joint3.set_position(roboArm.target.joint3.angle, 180);
    //         xSemaphoreGive(CAN1MutexHandle);
    //
    //         xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //         roboArm.joint4.set_position_near(roboArm.target.joint4.angle, 360);
    //         xSemaphoreGive(CAN1MutexHandle);
    //     }
    //     if ((cnt + 1) % 10 == 0) {
    //         xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //         roboArm.joint2.internal.set_position(roboArm.target.joint2.internal.angle, 180);
    //         roboArm.joint2.external.set_position(roboArm.target.joint2.external.angle, 180);
    //         xSemaphoreGive(CAN1MutexHandle);
    //     }
    //     if ((cnt + 2)%10 == 0) {
    //         xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //         roboArm.joint1.set_position(roboArm.target.joint1.angle, 180);
    //         xSemaphoreGive(CAN1MutexHandle);
    //
    //         // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //         // roboArm.joint4.read_totalposition();
    //         // xSemaphoreGive(CAN1MutexHandle);
    //     }
    //     if ((cnt+3) % 10 == 0) {
    //         // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //         // roboArm.joint3.read_totalposition();
    //         // xSemaphoreGive(CAN1MutexHandle);
    //
    //         // 相对旋转角度只需要一个电机就可以确定，所以只需要一个电机的反馈数据，这里选择外侧电机
    //         // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //         // roboArm.joint2.external.read_totalposition();
    //         // xSemaphoreGive(CAN1MutexHandle);
    //
    //         // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    //         // roboArm.joint1.read_totalposition();
    //         // roboArm.joint1.clear_error();
    //         // xSemaphoreGive(CAN1MutexHandle);
    //     }
    //     CANHeapCnt = uxTaskGetStackHighWaterMark(NULL);
    //     osDelay(1);
    // }
}
#include "RoboArm/RoboArm.hpp"