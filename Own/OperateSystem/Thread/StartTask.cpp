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

void robo_arm_e_callback(KeyEventType event);
void robo_arm_shift_e_callback(KeyEventType event);
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
void StartTask() {
    ada = SuperDWT::get_tick();
    /* USB初始化 */
    MX_USB_DEVICE_Init();

    /* 关闭两个24V  打开5V*/
    power_24v_right.WriteDown();
    power_24v_left.WriteDown();
    power_5v.WriteUp();

    /* W25Q64初始化 */
    w25q64.init();
    /* CAN初始化 */
    canPlus1.filter_config(1);
    canPlus2.filter_config(1);
    canPlus3.filter_config(2);
    canPlus1.start();
    canPlus2.start();
    canPlus3.start();
    /* 遥控器和键鼠初始化 */
    // 键鼠默认配置uint32_t clickTime = 75, uint32_t longPressTime = 1000
    interact.remote_control.detect.lostFun = &remote_ctrl_recover;
    KeyBoardRegister(interact.keyList, Key_W, CombineKey_None, chassis_w_callback);
    KeyBoardRegister(interact.keyList, Key_A, CombineKey_None, chassis_a_callback);
    KeyBoardRegister(interact.keyList, Key_S, CombineKey_None, chassis_s_callback);
    KeyBoardRegister(interact.keyList, Key_D, CombineKey_None, chassis_d_callback);
    KeyBoardRegister(interact.keyList, Key_Q, CombineKey_None, chassis_q_callback);

    KeyBoardRegister(interact.keyList, Key_Q, CombineKey_Shift, robo_arm_shift_q_callback);
    KeyBoardRegister(interact.keyList, Key_Q, CombineKey_Ctrl, robo_arm_ctrl_q_callback);
    KeyBoardRegister(interact.keyList, Key_Left, CombineKey_None, air_left_callback);
    interact.keyList[16].longPressTime = 100;
    KeyBoardRegister(interact.keyList, Key_Right, CombineKey_None, air_right_callback);
    interact.keyList[17].longPressTime = 5000;
    //    KeyBoardRegister(interact.keyList, Key_E, CombineKey_None, robo_arm_e_callback);
    //    KeyBoardRegister(interact.keyList, Key_E, CombineKey_Shift, robo_arm_shift_e_callback);
    interact.keyList[7].longPressTime = 5000;
    KeyBoardRegister(interact.keyList, Key_F, CombineKey_Shift, robo_arm_shift_f_callback);
    KeyBoardRegister(interact.keyList, Key_R, CombineKey_Shift, robo_arm_shift_r_callback);
    //    KeyBoardRegister(interact.keyList, Key_R, CombineKey_None, robo_arm_r_callback);

    KeyBoardRegister(interact.keyList, Key_Z, CombineKey_None, one_step_get_z_callback);
    KeyBoardRegister(interact.keyList, Key_Z, CombineKey_Shift, one_step_get_shift_z_callback);
    KeyBoardRegister(interact.keyList, Key_X, CombineKey_None, one_step_get_x_callback);
    KeyBoardRegister(interact.keyList, Key_X, CombineKey_Shift, one_step_get_shift_x_callback);
    KeyBoardRegister(interact.keyList, Key_C, CombineKey_None, one_step_get_c_callback);
    KeyBoardRegister(interact.keyList, Key_C, CombineKey_Shift, one_step_get_shift_c_callback);
    KeyBoardRegister(interact.keyList, Key_V, CombineKey_None, one_step_get_v_callback);
    KeyBoardRegister(interact.keyList, Key_V, CombineKey_Shift, one_step_get_shift_v_callback);

    interact.remote_control.start();
    interact.image_trans.uartPlus.receive_dma_idle(100);
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
