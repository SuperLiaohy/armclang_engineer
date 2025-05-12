//
// Created by Administrator on 25-2-22.
//

#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Buzzer/Buzzer.hpp"
#include "RoboArm/RoboArm.hpp"
#include "ThreadConfig.h"
extern uint8_t re_flag;

void ArmInitTask() {
    /* 机械臂电机使能 */
    // enable 内部是先失能再使能
    roboArm.enable();

    /* 机械臂的差分器初始化 */
    roboArm.diff.init(interact.joint);
    /* 读取电机的偏移量，来判断offset是否需要减少360 */
    roboArm.init_offset(interact.joint);

    roboArm.load_target(interact.joint, interact.joint_slope);
    interact.joint_slope[0].target_arrive();
    interact.joint_slope[1].target_arrive();
    interact.joint_slope[2].target_arrive();
    roboArm.update_relative_pos();

    xEventGroupWaitBits(osEventGroup, DIFF_LEFT_RECEIVE_EVENT | DIFF_RIGHT_RECEIVE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
    re_flag = 1;
    xEventGroupSetBits(osEventGroup, ROBO_ARM_INIT_END_EVENT);
		buzzer.PushMusic<24>(Buzzer::melody);
    vTaskDelete(NULL);
}