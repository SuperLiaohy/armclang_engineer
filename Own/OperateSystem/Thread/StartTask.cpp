//
// Created by liaohy on 24-11-15.
//

#include "CppTask.h"
#include "RoboArm/RoboArm.h"
#include <Motor/lkMotor.h>
#include <Motor/GM6020.h>
#include "Buzzer/Buzzer.h"
#include "Detect/Detect.h"
#include "CAN/SuperCan.h"
#include "Chassis/Chassis.h"
#include "RemoteControl/RemoteControl.h"
#include "W25Q64/W25Q64.h"
#include "RGBLED/RGBLED.h"
#include "ThreadConfig.h"
#include "Interact/Interact.h"
#ifdef __cplusplus
extern "C" {
#endif
extern osThreadId ERROR_TASKHandle;
#include "usb_device.h"
#ifdef __cplusplus
}
#endif


void StartTask() {
    /* 使能两个24V 和 5V*/
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
    /* USB初始化 */
    MX_USB_DEVICE_Init();

    /* W25Q64初始化 */
    w25q64.init();
    /* CAN初始化 */
    canPlus1.filter_config(0);
    canPlus2.filter_config(1);
    canPlus1.can_start();
    canPlus2.can_start();
    /* 底盘电机初始化 */
    chassis.base.left_front.motor.init(1,15,0,4,8000,16000,19.2);
    chassis.base.right_front.motor.init(2,15,0,4,8000,16000,19.2);
    chassis.base.left_rear.motor.init(3,15,0,4,8000,16000,19.2);
    chassis.base.right_rear.motor.init(4,15,0,4,8000,16000,19.2);
    chassis.extend.right.motor.init(4,15,0,4,8000,16000,19.2);
    chassis.extend.left.motor.init(4,15,0,4,8000,16000,19.2);
    /* 机械臂电机初始化 */
    roboArm.joint1.motor.init(&canPlus1,10);
    roboArm.joint2.motor.init(&canPlus1,6);
    roboArm.joint3.motor.init(&canPlus1,6);
    roboArm.joint4.motor.init(&canPlus1,10);
    /* 机械臂末端差分器的初始化 */
    roboArm.diff.left.motor.init(1,
                                 20.f, 0.01f, 1.f, 2000.f, 10000.0f,
                                 60, 0.05, 2, 160, 800, motor_const::M2006Gain);
    roboArm.diff.left.motor.doublePid.extern_pid.dead_zone = 0.0;
    roboArm.diff.right.motor.init(2,
                                  20.f, 0.01f, 1.f, 2000.f, 10000.0f,
                                  60, 0.05, 2, 160, 800, motor_const::M2006Gain);
    roboArm.diff.right.motor.doublePid.extern_pid.dead_zone = 0.0;
    test_motor.motor.init(0.6f, 0.01f, 0.1f, 500.f, 500.0f,
                          5, 0.05f, 1, 900, 900);
    /* 机械臂电机使能 */
    roboArm.enable();

//    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    /* 读取电机的偏移量，来判断offset是否需要减少360 */
//    roboArm.init_offset(interact);
//    xSemaphoreGive(CAN1MutexHandle);

    /* 遥控器初始化 */
    remote_control.detect.lostFun = &remote_ctrl_recover;
    remote_control.KeyBoardRegister(Key_W, CombineKey_None, chassis_w_callback);
    remote_control.KeyBoardRegister(Key_A, CombineKey_None, chassis_a_callback);
    remote_control.KeyBoardRegister(Key_S, CombineKey_None, chassis_s_callback);
    remote_control.KeyBoardRegister(Key_D, CombineKey_None, chassis_d_callback);
    remote_control.start();

    /* 闪烁灯初始化 (暂无)*/

    /* 蜂鸣器初始化 */
    buzzer.Start();

    /* 判断急停 */
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    /* 判断can线上的设备是否初始化完毕 */
    xEventGroupSetBits(osEventGroup, CAN_RECEIVE_EVENT);
    xEventGroupWaitBits(osEventGroup, CAN_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    /* 机械臂的差分器初始化 */
    roboArm.diff.init();

    xEventGroupSetBits(osEventGroup, START_END_EVENT);

    StartHeapCnt = uxTaskGetStackHighWaterMark(NULL);
    vTaskDelete(NULL);
    while (1) {};
}