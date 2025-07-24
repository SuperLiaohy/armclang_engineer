//
// Created by Administrator on 25-2-22.
//

#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Buzzer/Buzzer.hpp"
#include "RoboArm/RoboArm.hpp"
#include "ThreadConfig.h"
extern uint8_t re_flag;
extern std::atomic<bool> arminit_flag;
void ArmInitTask() {
    /* 机械臂电机使能 */
    // enable 内部是先失能再使能
    roboArm.enable();

    /* 读取电机的偏移量，来判断offset是否需要减少360 */
    roboArm.init_offset(interact.joint);

    roboArm.load_target(interact.joint, interact.joint_slope);
    for (auto & item: interact.joint_slope) {
        item.target_arrive();
    }
    roboArm.update_relative_pos();

    xEventGroupSetBits(osEventGroup, ROBO_ARM_INIT_END_EVENT);
    buzzer.CleanMusic();
    buzzer.PushMusic<24>(Buzzer::melody);
    arminit_flag.store(true);

    ARM_INIT_TASKHandle = NULL;
    vTaskDelete(NULL);
}