//
// Created by Administrator on 25-2-22.
//

#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "RoboArm/RoboArm.hpp"
#include "ThreadConfig.h"
extern uint8_t re_flag;

void ArmInitTask() {
    /* 机械臂电机使能 */
    // enable 内部是先失能再使能
    roboArm.enable();

    /* 读取电机的偏移量，来判断offset是否需要减少360 */
    roboArm.init_offset(interact);

    xEventGroupWaitBits(osEventGroup, DIFF_LEFT_RECEIVE_EVENT | DIFF_RIGHT_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    /* 机械臂的差分器初始化 */
    roboArm.diff.init();
    re_flag = 1;
    xEventGroupSetBits(osEventGroup, ROBO_ARM_INIT_END_EVENT);
		buzzer.PushMusic<24>(Buzzer::melody);
    vTaskDelete(NULL);
}