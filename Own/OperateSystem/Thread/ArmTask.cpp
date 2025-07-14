//
// Created by Administrator on 2025/4/4.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "RoboArm/RoboArm.hpp"
#include "Imu/Imu.hpp"

void ArmTask() {
    uint32_t cnt = 0;

    while (true) {
        ++cnt;

        interact.receive_actions_group();
        interact.receive_actions(roboArm, imu.pitch);
        interact.receive_kb();
        roboArm.update_relative_pos();
        roboArm.load_target(interact.joint, interact.joint_slope);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        // roboArm.joint5.read_feedback();
        roboArm.joint5.set_position(roboArm.target.joint5.angle/100.f);
        roboArm.joint5.SingleControl();
        xSemaphoreGive(CAN1MutexHandle);
        if ((cnt + 4) % 5 == 0) {
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            // roboArm.joint6.read_feedback();
            roboArm.joint6.set_position(roboArm.target.joint6.angle, 720);
            xSemaphoreGive(CAN1MutexHandle);
        }
        if ((cnt) % 5 == 0) {
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint3.set_position(roboArm.target.joint3.angle, roboArm.target_speed[2]);
            xSemaphoreGive(CAN1MutexHandle);

            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            // roboArm.joint4.set_position_near(roboArm.target.joint4.angle, roboArm.target_speed[3]);
            roboArm.joint4.set_position(roboArm.target.joint4.angle, roboArm.target_speed[3]);
            xSemaphoreGive(CAN1MutexHandle);
        }
        if ((cnt + 1) % 5 == 0) {
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint2.internal.set_position(roboArm.target.joint2.internal.angle, roboArm.target_speed[1]);
            roboArm.joint2.external.set_position(roboArm.target.joint2.external.angle, roboArm.target_speed[1]);
            xSemaphoreGive(CAN1MutexHandle);
        }
        if ((cnt + 2) % 5 == 0) {
            xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            roboArm.joint1.set_position(roboArm.target.joint1.angle, roboArm.target_speed[0]);
            // roboArm.joint1.read_feedback();
            xSemaphoreGive(CAN1MutexHandle);

            // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            // roboArm.joint4.read_totalposition();
            // xSemaphoreGive(CAN1MutexHandle);
        }
        if ((cnt + 3) % 10 == 0) {
            // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            // roboArm.joint3.read_totalposition();
            // xSemaphoreGive(CAN1MutexHandle);

            // 相对旋转角度只需要一个电机就可以确定，所以只需要一个电机的反馈数据，这里选择外侧电机
            // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            // roboArm.joint2.external.read_totalposition();
            // xSemaphoreGive(CAN1MutexHandle);

            // xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
            // roboArm.joint1.read_totalposition();
            // xSemaphoreGive(CAN1MutexHandle);
        }
        CANHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelay(1);
    }
}