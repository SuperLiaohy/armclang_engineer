//
// Created by Administrator on 24-12-25.
//

#include "CppTask.h"
#include "Vofa/vofa.h"
#include "Motor/lkMotor.h"
#include "Motor/GM6020.h"
#include "Uart/SuperUart.h"
#include "CDC/SuperCDC.h"
#include "Imu/Imu.h"
#include "RoboArm/RoboArm.h"
#include "Interact/Interact.h"
#include "MicroTime/MicroTime.h"

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
//        lkMotorTime.start();
        roboArm.load_target(interact);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint4.motor.totalposition2Control(480,roboArm.target.joint4.angle * roboArm.joint4.motor.gain);
        xSemaphoreGive(CAN1MutexHandle);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint3.motor.totalposition2Control(180,roboArm.target.joint3.angle * roboArm.joint3.motor.gain);
        xSemaphoreGive(CAN1MutexHandle);
//
////        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
////        roboArm.joint2.motor.totalposition2Control(180 ,roboArm.target.joint2.angle * roboArm.joint2.motor.gain);
////        xSemaphoreGive(CAN1MutexHandle);
        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint2.internal.motor.totalposition2Control(180 ,roboArm.target.joint2.internal.angle * roboArm.joint2.internal.motor.gain);
        xSemaphoreGive(CAN1MutexHandle);
        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint2.external.motor.totalposition2Control(180 ,roboArm.target.joint2.external.angle * roboArm.joint2.external.motor.gain);
        xSemaphoreGive(CAN1MutexHandle);

        lkMotorTime.end();
        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint1.motor.totalposition2Control(540, roboArm.target.joint1.angle * roboArm.joint1.motor.gain);
        xSemaphoreGive(CAN1MutexHandle);
        osDelay(1);
        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint4.motor.read_totalposition();
        xSemaphoreGive(CAN1MutexHandle);

        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint3.motor.read_totalposition();
        xSemaphoreGive(CAN1MutexHandle);
osDelay(1);
//        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
//        roboArm.joint2.motor.read_totalposition();
//        xSemaphoreGive(CAN1MutexHandle);
        // 相对旋转角度只需要一个电机就可以确定，所以只需要一个电机的反馈数据，这里选择外侧电机
        xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
        roboArm.joint2.external.motor.read_totalposition();
        xSemaphoreGive(CAN1MutexHandle);
				
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