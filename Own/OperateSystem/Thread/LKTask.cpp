//
// Created by Administrator on 24-12-25.
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

#include "LetterShell/cpp_support/shell_cpp.h"
#ifdef __cplusplus
extern "C" {
#endif
extern Shell shell;
#include "LetterShell/log/log.h"
#ifdef __cplusplus
}
#endif

MicroTime lkMotorTime;
void LKTask() {
//    osDelay(1000);

    osDelay(15);
    while (1) {

        auto now = osKernelSysTick();

        roboArm.load_target(interact);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint3.motor.set_position(roboArm.target.joint3.angle, 600);
        xSemaphoreGive(CAN1MutexHandle);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint4.motor.set_position(roboArm.target.joint4.angle, 720);
        xSemaphoreGive(CAN1MutexHandle);

        osDelay(1);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint2.internal.motor.set_position(roboArm.target.joint2.internal.angle, 360);
        roboArm.joint2.external.motor.set_position(roboArm.target.joint2.external.angle, 360);
        xSemaphoreGive(CAN1MutexHandle);

        osDelay(1);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint1.motor.set_position(roboArm.target.joint1.angle, 600);
        xSemaphoreGive(CAN1MutexHandle);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint4.motor.read_totalposition();
        xSemaphoreGive(CAN1MutexHandle);

        osDelay(1);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint3.motor.read_totalposition();
        xSemaphoreGive(CAN1MutexHandle);

//        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
//        roboArm.joint2.motor.read_totalposition();
//        xSemaphoreGive(CAN1MutexHandle);
        // 相对旋转角度只需要一个电机就可以确定，所以只需要一个电机的反馈数据，这里选择外侧电机
        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint2.external.motor.read_totalposition();
        xSemaphoreGive(CAN1MutexHandle);
        osDelay(1);
//        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
//        roboArm.joint2.internal.motor.read_totalposition();
//        xSemaphoreGive(CAN1MutexHandle);
        
        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint1.motor.read_totalposition();
        xSemaphoreGive(CAN1MutexHandle);

        CANHeapCnt = uxTaskGetStackHighWaterMark(NULL);

        osDelayUntil(&now, 10);

    }

}