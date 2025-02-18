//
// Created by liaohy on 24-11-15.
//

#include "Buzzer/Buzzer.h"
#include "CAN/SuperCan.h"
#include "Chassis/Chassis.h"
#include "CppTask.h"
#include "Detect/Detect.h"
#include "Interact/Interact.h"
#include "Pump/Pump.h"
#include "RGBLED/RGBLED.h"
#include "RemoteControl/RemoteControl.h"
#include "RoboArm/RoboArm.h"
#include "ThreadConfig.h"
#include "W25Q64/W25Q64.h"
#include <Motor/GM6020.h>
#include <Motor/lkMotor.h>

void air_left_callback(KeyEventType event);
void air_right_callback(KeyEventType event);

void robo_arm_e_callback(KeyEventType event);
void robo_arm_shift_e_callback(KeyEventType event);
void robo_arm_shift_q_callback(KeyEventType event);
void robo_arm_shift_c_callback(KeyEventType event);
void robo_arm_shift_f_callback(KeyEventType event);

void chassis_w_callback(KeyEventType event);
void chassis_a_callback(KeyEventType event);
void chassis_s_callback(KeyEventType event);
void chassis_d_callback(KeyEventType event);
void chassis_q_callback(KeyEventType event);


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
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
    //    power_24v_left.WriteDown();
    /* USB初始化 */
    MX_USB_DEVICE_Init();

    test_actions.joint1.data[0].angle = 0;
    test_actions.joint2.data[0].angle = -50;
    test_actions.joint3.data[0].angle = 135;
    test_actions.joint4.data[0].angle = 0;
    test_actions.joint5.data[0].angle = -90;
    test_actions.joint6.data[0].angle = 0;

    test_actions.joint1.data[1].angle = 0;
    test_actions.joint2.data[1].angle = -25;
    test_actions.joint3.data[1].angle = 75;
    test_actions.joint4.data[1].angle = 0;
    test_actions.joint5.data[1].angle = -70;
    test_actions.joint6.data[1].angle = 0;

    test_actions.joint1.data[2].angle = 0;
    test_actions.joint2.data[2].angle = -0;
    test_actions.joint3.data[2].angle = 0;
    test_actions.joint4.data[2].angle = 0;
    test_actions.joint5.data[2].angle = -30;
    test_actions.joint6.data[2].angle = 0;

    test_actions.joint1.data[3].angle = 0;
    test_actions.joint2.data[3].angle = 30;
    test_actions.joint3.data[3].angle = -30;
    test_actions.joint4.data[3].angle = 0;
    test_actions.joint5.data[3].angle = -0;
    test_actions.joint6.data[3].angle = 0;

    pump.close();

    /* W25Q64初始化 */
    w25q64.init();
    /* CAN初始化 */
    canPlus1.filter_config(0);
    canPlus2.filter_config(1);
    canPlus1.can_start();
    canPlus2.can_start();
    /* 遥控器和键鼠初始化 */
    // 键鼠默认配置uint32_t clickTime = 75, uint32_t longPressTime = 1000
    interact.remote_control.detect.lostFun = &remote_ctrl_recover;
    KeyBoardRegister(interact.keyList, Key_W, CombineKey_None, chassis_w_callback);
    KeyBoardRegister(interact.keyList, Key_A, CombineKey_None, chassis_a_callback);
    KeyBoardRegister(interact.keyList, Key_S, CombineKey_None, chassis_s_callback);
    KeyBoardRegister(interact.keyList, Key_D, CombineKey_None, chassis_d_callback);
    KeyBoardRegister(interact.keyList, Key_Q, CombineKey_None, chassis_q_callback);
    KeyBoardRegister(interact.keyList, Key_Q, CombineKey_Shift, robo_arm_shift_q_callback);
    KeyBoardRegister(interact.keyList, Key_Left, CombineKey_None, air_left_callback);
    interact.keyList[16].longPressTime = 100;
    KeyBoardRegister(interact.keyList, Key_Right, CombineKey_None, air_right_callback);
    interact.keyList[17].longPressTime = 5000;
    KeyBoardRegister(interact.keyList, Key_E, CombineKey_None, robo_arm_e_callback);
    KeyBoardRegister(interact.keyList, Key_E, CombineKey_Shift, robo_arm_shift_e_callback);
    interact.keyList[7].longPressTime = 5000;
    KeyBoardRegister(interact.keyList, Key_C, CombineKey_Shift, robo_arm_shift_c_callback);
    KeyBoardRegister(interact.keyList, Key_R, CombineKey_Shift, robo_arm_shift_f_callback);
    interact.remote_control.start();
    interact.image_trans.uartPlus.read_idle(100);

    /* 底盘电机初始化 */
    chassis.base.left_front.motor.init(15, 0, 4, 8000, 16000, 19.2);
    chassis.base.right_front.motor.init(15, 0, 4, 8000, 16000, 19.2);
    chassis.base.left_rear.motor.init(15, 0, 4, 8000, 16000, 19.2);
    chassis.base.right_rear.motor.init(15, 0, 4, 8000, 16000, 19.2);
    chassis.extend.right.motor.init(15, 0, 4, 8000, 16000, 19.2);
    chassis.extend.left.motor.init(15, 0, 4, 8000, 16000, 19.2);
    /* 机械臂电机初始化 */
    roboArm.joint1.motor.init(&canPlus1, 10);
    //    roboArm.joint2.motor.init(&canPlus1,6);
    roboArm.joint2.internal.motor.init(&canPlus1, 6);
    roboArm.joint2.external.motor.init(&canPlus1, 6);
    roboArm.joint3.motor.init(&canPlus1, 6);
    roboArm.joint4.motor.init(&canPlus1, 10);
    /* 机械臂末端差分器的初始化 */
    roboArm.diff.left.motor.init(2.f, 0.01f, 0.2f, 2000.f, 10000.0f,
                                 60 * 4, 0.05, 5, 1600, 8000, motor_const::M2006Gain);
    roboArm.diff.left.motor.doublePid.extern_pid.dead_zone = 0.0;
    roboArm.diff.right.motor.init(2.f, 0.01f, 0.2f, 2000.f, 10000.0f,
                                  60 * 4, 0.05, 5, 1600, 8000, motor_const::M2006Gain);
    roboArm.diff.right.motor.doublePid.extern_pid.dead_zone = 0.0;

    test_motor.motor.init(0.6f, 0.01f, 0.1f, 500.f, 500.0f,
                          5, 0.05f, 1, 900, 900);

    /* 蜂鸣器初始化 */
    buzzer.Start();
    /* 判断急停 */
    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);

    /* 机械臂电机使能 */
    // enable 内部是先失能再使能
    roboArm.enable();

    /* 读取电机的偏移量，来判断offset是否需要减少360 */
    roboArm.init_offset(interact);

    /* 闪烁灯初始化 (暂无)*/

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