//
// Created by liaohy on 24-11-15.
//

#include "Buzzer/Buzzer.hpp"
#include "CAN/SuperCan.hpp"
#include "Chassis/Chassis.hpp"
#include "CppTask.hpp"
#include "Detect/Detect.hpp"
#include "Interact/Interact.hpp"
#include "Judge/ui.hpp"
#include "Pump/Pump.hpp"
#include "RGBLED/RGBLED.hpp"
#include "RemoteControl/RemoteControl.hpp"
#include "RoboArm/RoboArm.hpp"
#include "ThreadConfig.h"
#include "W25Q64/W25Q64.hpp"
#include <DWT/SuperDWT.hpp>

void air_left_callback(KeyEventType event);
void air_right_callback(KeyEventType event);

void chassis_shift_e_callback(KeyEventType event);
void robo_arm_shift_q_callback(KeyEventType event);
void robo_arm_ctrl_q_callback(KeyEventType event);
void robo_arm_shift_f_callback(KeyEventType event);
void robo_arm_r_callback(KeyEventType event);
void robo_arm_shift_r_callback(KeyEventType event);

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
#include "WDG/SuperIWDG.hpp"
uint8_t re_flag       = 0;
volatile uint32_t ada = 0;
void one_step_get_z_callback(KeyEventType event);
void one_step_get_shift_z_callback(KeyEventType event);
void one_step_get_x_callback(KeyEventType event);
void one_step_get_shift_v_callback(KeyEventType event);
void one_step_get_v_callback(KeyEventType event);
void one_step_get_shift_c_callback(KeyEventType event);
void one_step_get_c_callback(KeyEventType event);
void one_step_get_shift_x_callback(KeyEventType event);

void one_step_get_ctrl_z_callback(KeyEventType event);
void one_step_get_ctrl_x_callback(KeyEventType event);
void one_step_get_ctrl_c_callback(KeyEventType event);
void one_step_get_ctrl_v_callback(KeyEventType event);

void robo_arm_ctrl_e_callback(KeyEventType event);
void robo_arm_ctrl_r_callback(KeyEventType event);

extern interact_dep::Actions reset1;
extern interact_dep::Actions reset2;
extern interact_dep::Actions silver_reset;

extern interact_dep::Actions anti_reset;
extern interact_dep::Actions get_right_y;
extern interact_dep::Actions get_silver_mine;
extern interact_dep::Actions get_silver_mine_z;
extern interact_dep::Actions put_silver_mine_right;
extern interact_dep::Actions put_silver_mine_left;
extern interact_dep::Actions get_silver_from_left;
extern interact_dep::Actions exchange_left;
extern interact_dep::Actions exchange_right;
extern interact_dep::Actions put_down;

extern interact_dep::ActionsGroup get_silver_group;
extern interact_dep::ActionsGroup get_second_silver_group;
extern interact_dep::ActionsGroup get_silver_from_left_group;
extern interact_dep::ActionsGroup get_gold_group;
extern interact_dep::ActionsGroup put_down_group;

void chassis_shift_w_callback(KeyEventType event);
void chassis_shift_a_callback(KeyEventType event);
void chassis_shift_s_callback(KeyEventType event);
void chassis_shift_d_callback(KeyEventType event);
void one_step_get_b_callback(KeyEventType event);
void robo_arm_ctrl_g_callback(KeyEventType event);
void robo_arm_ctrl_f_callback(KeyEventType event);
void robo_arm_e_callback(KeyEventType event);
void robo_arm_f_callback(KeyEventType event);
void robo_arm_g_callback(KeyEventType event);

void chassis_motor_detect() {
    buzzer.PushMusic<8>(Buzzer::error_music);
}

void joint1_motor_detect() {
    buzzer.PushMusic<8>(Buzzer::error_music);
    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint1.clear_error();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint1.disable();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint1.enable();
    xSemaphoreGive(CAN1MutexHandle);
}

void joint2_internal_motor_detect() {
    buzzer.PushMusic<8>(Buzzer::error_music);
    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint2.internal.clear_error();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint2.internal.disable();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint2.internal.enable();
    xSemaphoreGive(CAN1MutexHandle);
}
void joint2_external_motor_detect() {
    buzzer.PushMusic<8>(Buzzer::error_music);
    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint2.external.clear_error();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint2.external.disable();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint2.external.enable();
    xSemaphoreGive(CAN1MutexHandle);
}
void joint3_motor_detect() {
    buzzer.PushMusic<8>(Buzzer::error_music);
    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint3.clear_error();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint3.disable();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint3.enable();
    xSemaphoreGive(CAN1MutexHandle);
}
void joint4_motor_detect() {
    buzzer.PushMusic<8>(Buzzer::error_music);
    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint4.clear_error();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint4.disable();
    xSemaphoreGive(CAN1MutexHandle);

    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    roboArm.joint4.enable();
    xSemaphoreGive(CAN1MutexHandle);
}
void StartTask() {
    ada = SuperDWT::get_tick();
    /* USB初始化 */
    MX_USB_DEVICE_Init();

    /* 关闭两个24V  打开5V*/
    power_24v_right.WriteDown();
    power_24v_left.WriteDown();
    power_5v.WriteUp();

    chassis.base.left_front.detect_lost(chassis_motor_detect);
    chassis.base.right_front.detect_lost(chassis_motor_detect);
    chassis.base.left_rear.detect_lost(chassis_motor_detect);
    chassis.base.right_rear.detect_lost(chassis_motor_detect);
    chassis.extend.left.detect_lost(chassis_motor_detect);
    chassis.extend.right.detect_lost(chassis_motor_detect);

    roboArm.joint1.detect_lost(joint1_motor_detect);
    roboArm.joint2.internal.detect_lost(joint2_internal_motor_detect);
    roboArm.joint2.external.detect_lost(joint2_external_motor_detect);
    roboArm.joint3.detect_lost(joint3_motor_detect);
    roboArm.joint4.detect_lost(joint4_motor_detect);

    reset1.joints[0] = 0;
    reset1.joints[1] = -55;
    reset1.joints[2] = 145;
    reset1.joints[3] = 0;
    reset1.joints[4] = 0;
    reset1.joints[5] = 0;

    reset1.speed[0] = 720;
    reset1.speed[1] = 720;
    reset1.speed[2] = 360;
    reset1.speed[3] = 720;

    reset2.joints[0] = 0;
    reset2.joints[1] = -4;
    reset2.joints[2] = 145;
    reset2.joints[3] = 0;
    reset2.joints[4] = 0;
    reset2.joints[5] = 0;

    reset2.speed[0] = 720;
    reset2.speed[1] = 720;
    reset2.speed[2] = 360;
    reset2.speed[3] = 720;

    silver_reset.joints[0] = 0;
    silver_reset.joints[1] = -55;
    silver_reset.joints[2] = 135;
    silver_reset.joints[3] = 0;
    silver_reset.joints[4] = 0;
    silver_reset.joints[5] = 0;

    silver_reset.speed[0] = 720;
    silver_reset.speed[1] = 180;
    silver_reset.speed[2] = 360;
    silver_reset.speed[3] = 360;

    // reset2.joints[0] = 0;
    // reset2.joints[1] = -55;
    // reset2.joints[2] = 125;
    // reset2.joints[3] = 0;
    // reset2.joints[4] = 0;
    // reset2.joints[5] = 0;
    //
    // reset2.speed[0] = 720;
    // reset2.speed[1] = 720;
    // reset2.speed[2] = 360;
    // reset2.speed[3] = 720;

    anti_reset.joints[0] = -3.35253143;
    anti_reset.joints[1] = -45.0083809;
    anti_reset.joints[2] = -62.4916077;
    anti_reset.joints[3] = 0.0380706787;
    anti_reset.joints[4] = 0.667541504;
    anti_reset.joints[5] = 0.699188232;

    SuperIWDG::GotInstance().give();


    get_right_y.joints[0] = -8.48937225;
    get_right_y.joints[1] = -17.2843819;
    get_right_y.joints[2] = -102.246674;
    get_right_y.joints[3] = 83.5459137;
    get_right_y.joints[4] = 89.6870422;
    get_right_y.joints[5] = 42.9085999;

    get_silver_mine.joints[0] = 0;
    get_silver_mine.joints[1] = 34.3415947;
    get_silver_mine.joints[2] = 121.3975;
    get_silver_mine.joints[3] = 0;
    get_silver_mine.joints[4] = 24.3;
    get_silver_mine.joints[5] = 0;

    put_silver_mine_right.joints[0] = 16.20;
    put_silver_mine_right.joints[1] = 14.4343681;
    put_silver_mine_right.joints[2] = 122.649994;
    put_silver_mine_right.joints[3] = 101.64;
    put_silver_mine_right.joints[4] = 90.78;
    put_silver_mine_right.joints[5] = 38.22;

    put_down.joints[0] = 0;
    put_down.joints[1] = 55;
    put_down.joints[2] = 116.5;
    put_down.joints[3] = 0;
    put_down.joints[4] = -78;
    put_down.joints[5] = 0;

    // put_silver_mine_left.joints[0] = -44.8466797;
    // put_silver_mine_left.joints[1] = 1.91878128;
    // put_silver_mine_left.joints[2] = 108.428192;
    // put_silver_mine_left.joints[3] = -0.359306335;
    // put_silver_mine_left.joints[4] = 73.6503601;
    // put_silver_mine_left.joints[5] = 40.1804858;

    put_silver_mine_left.joints[0] = -53.1208191;
    put_silver_mine_left.joints[1] = 5.90897751;
    put_silver_mine_left.joints[2] = 101.512299;
    put_silver_mine_left.joints[3] = 0.601997375;
    put_silver_mine_left.joints[4] = 83.9671173;
    put_silver_mine_left.joints[5] = 35.4749832;

    put_silver_mine_left.speed[0] = 720;
    put_silver_mine_left.speed[1] = 180;
    put_silver_mine_left.speed[2] = 180;
    put_silver_mine_left.speed[3] = 720;

    get_silver_from_left.joints[0] = -27.51478;
    get_silver_from_left.joints[1] = 10.4793777;
    get_silver_from_left.joints[2] = 126.511063;
    get_silver_from_left.joints[3] = 95.8899307;
    get_silver_from_left.joints[4] = -90;
    get_silver_from_left.joints[5] = -7.86681938;

    get_silver_from_left.speed[0] = 90;
    get_silver_from_left.speed[1] = 180;
    get_silver_from_left.speed[2] = 180;
    get_silver_from_left.speed[3] = 720;

    exchange_left.joints[0] = -17.9960938;
    exchange_left.joints[1] = 36.7366142;
    exchange_left.joints[2] = 35.3361511;
    exchange_left.joints[3] = -89.4694138;
    exchange_left.joints[4] = -89.9465207;
    exchange_left.joints[5] = 52.6248474;

    exchange_right.joints[0] = 17.9960938;
    exchange_right.joints[1] = 36.7366142;
    exchange_right.joints[2] = 35.3361511;
    exchange_right.joints[3] = 89.4694138;
    exchange_right.joints[4] = -89.9465207;
    exchange_right.joints[5] = -52.6248474;

    get_silver_group.actions_list[0] = get_silver_mine;
    get_silver_group.actions_list[1] = get_silver_mine_z;
    get_silver_group.actions_list[2] = put_silver_mine_left;
    get_silver_group.actions_list[3] = put_silver_mine_left;
    get_silver_group.actions_list[4] = exchange_left;
    get_silver_group.actions_list[4].speed[0] = 180;
    get_silver_group.actions_list[4].speed[1] = 180;
    get_silver_group.actions_list[4].speed[2] = 180;
    get_silver_group.actions_list[4].speed[3] = 180;
    get_silver_group.actions_list[5] = silver_reset;

    get_second_silver_group.actions_list[0] = get_silver_mine;
    get_second_silver_group.actions_list[1] = get_silver_mine_z;
    get_second_silver_group.actions_list[2] = reset1;

    get_silver_from_left_group.actions_list[0] = get_silver_from_left;
    get_silver_from_left_group.actions_list[1] = reset1;

    get_gold_group.actions_list[0] = reset1;
    get_gold_group.actions_list[1] = reset1;
    get_gold_group.actions_list[2] = reset1;
    get_gold_group.actions_list[3] = reset1;

    put_down_group.actions_list[0] = put_down;
    put_down_group.actions_list[1] = reset2;


    /* W25Q64初始化 */
    w25q64.init();
    /* CAN初始化 */
    canPlus1.filter_config(1);
    canPlus2.filter_config(1);
    canPlus3.filter_config(2);
    canPlus1.start();
    canPlus2.start();
    canPlus3.start();
    SuperIWDG::GotInstance().give();
    /* 遥控器和键鼠初始化 */
    // 键鼠默认配置uint32_t clickTime = 75, uint32_t longPressTime = 1000
    interact.remote_control.detect.lostFun = &remote_ctrl_recover;
    KeyBoardRegister(interact.keyList, Key_W, CombineKey_None, chassis_w_callback);
    KeyBoardRegister(interact.keyList, Key_A, CombineKey_None, chassis_a_callback);
    KeyBoardRegister(interact.keyList, Key_S, CombineKey_None, chassis_s_callback);
    KeyBoardRegister(interact.keyList, Key_D, CombineKey_None, chassis_d_callback);

    KeyBoardRegister(interact.keyList, Key_W, CombineKey_Shift, chassis_shift_w_callback);
    KeyBoardRegister(interact.keyList, Key_A, CombineKey_Shift, chassis_shift_a_callback);
    KeyBoardRegister(interact.keyList, Key_S, CombineKey_Shift, chassis_shift_s_callback);
    KeyBoardRegister(interact.keyList, Key_D, CombineKey_Shift, chassis_shift_d_callback);

    KeyBoardRegister(interact.keyList, Key_Q, CombineKey_None, chassis_q_callback);

    KeyBoardRegister(interact.keyList, Key_Q, CombineKey_Shift, robo_arm_shift_q_callback);
    KeyBoardRegister(interact.keyList, Key_Q, CombineKey_Ctrl, robo_arm_ctrl_q_callback);

    KeyBoardRegister(interact.keyList, Key_Left, CombineKey_None, air_left_callback);
    interact.keyList[16].longPressTime = 100;
    KeyBoardRegister(interact.keyList, Key_Right, CombineKey_None, air_right_callback);
    interact.keyList[16].longPressTime = 5000;

    //    KeyBoardRegister(interact.keyList, Key_E, CombineKey_None, robo_arm_e_callback);
    //    KeyBoardRegister(interact.keyList, Key_E, CombineKey_Shift, robo_arm_shift_e_callback);
    interact.keyList[7].longPressTime = 5000;
    // KeyBoardRegister(interact.keyList, Key_F, CombineKey_Shift, robo_arm_shift_f_callback);
    KeyBoardRegister(interact.keyList, Key_R, CombineKey_None, robo_arm_r_callback);
    KeyBoardRegister(interact.keyList, Key_R, CombineKey_Shift, robo_arm_shift_r_callback);
    KeyBoardRegister(interact.keyList, Key_R, CombineKey_Ctrl, robo_arm_ctrl_r_callback);
    //    KeyBoardRegister(interact.keyList, Key_R, CombineKey_None, robo_arm_r_callback);


    KeyBoardRegister(interact.keyList, Key_F, CombineKey_Shift, robo_arm_shift_f_callback);
    KeyBoardRegister(interact.keyList, Key_F, CombineKey_Ctrl, robo_arm_ctrl_f_callback);

    KeyBoardRegister(interact.keyList, Key_E, CombineKey_None, robo_arm_e_callback);
    KeyBoardRegister(interact.keyList, Key_F, CombineKey_None, robo_arm_f_callback);

    KeyBoardRegister(interact.keyList, Key_E, CombineKey_Ctrl, robo_arm_ctrl_e_callback);
    KeyBoardRegister(interact.keyList, Key_E, CombineKey_Shift, chassis_shift_e_callback);

    KeyBoardRegister(interact.keyList, Key_Z, CombineKey_None, one_step_get_z_callback);
    KeyBoardRegister(interact.keyList, Key_Z, CombineKey_Shift, one_step_get_shift_z_callback);
    KeyBoardRegister(interact.keyList, Key_X, CombineKey_None, one_step_get_x_callback);
    KeyBoardRegister(interact.keyList, Key_X, CombineKey_Shift, one_step_get_shift_x_callback);
    KeyBoardRegister(interact.keyList, Key_C, CombineKey_None, one_step_get_c_callback);
    KeyBoardRegister(interact.keyList, Key_C, CombineKey_Shift, one_step_get_shift_c_callback);
    KeyBoardRegister(interact.keyList, Key_V, CombineKey_None, one_step_get_v_callback);
    KeyBoardRegister(interact.keyList, Key_V, CombineKey_Shift, one_step_get_shift_v_callback);

    KeyBoardRegister(interact.keyList, Key_Z, CombineKey_Ctrl, one_step_get_ctrl_z_callback);
    KeyBoardRegister(interact.keyList, Key_X, CombineKey_Ctrl, one_step_get_ctrl_x_callback);
    KeyBoardRegister(interact.keyList, Key_C, CombineKey_Ctrl, one_step_get_ctrl_c_callback);
    KeyBoardRegister(interact.keyList, Key_V, CombineKey_Ctrl, one_step_get_ctrl_v_callback);

    KeyBoardRegister(interact.keyList, Key_B, CombineKey_None, one_step_get_b_callback);
    KeyBoardRegister(interact.keyList, Key_G, CombineKey_Ctrl, robo_arm_ctrl_g_callback);
    KeyBoardRegister(interact.keyList, Key_G, CombineKey_None, robo_arm_g_callback);


    interact.remote_control.start();
    interact.image_trans.start_receive();
    interact.sub_board.start_receive();
    ui.start_receive();

    /* 蜂鸣器初始化 */
    buzzer.Start();
    ada = SuperDWT::get_tick() - ada;

    xEventGroupWaitBits(osEventGroup, REMOTE_CONTROL_START_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupSetBits(osEventGroup, START_END_EVENT);
    StartHeapCnt = uxTaskGetStackHighWaterMark(NULL);
    vTaskDelete(NULL);
}
