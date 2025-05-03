//
// Created by liaohy on 24-11-15.
//

#include "Chassis/Chassis.hpp"
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "MicroTime/MicroTime.hpp"
#include "ThreadConfig.h"
#include <RemoteControl/RemoteControl.hpp>

#ifdef __cplusplus
extern "C" {
#endif

extern osThreadId ERROR_TASKHandle;

#ifdef __cplusplus
}
#endif

MicroTime kineTime;
extern uint8_t re_flag;
void RemoteCtrlTask() {
    using namespace my_math;
    using namespace roboarm_dep;
    using namespace remote_ctrl_dep;
    const auto& rc = interact.remote_control.rcInfo;
    while (1) {
        auto now = osKernelSysTick();
        if (!interact.remote_control.detect.isLost) {
            if (rc.left != static_cast<uint8_t>(lever::lower) && rc.right != static_cast<uint8_t>(lever::lower)) {
                if (rc.wheel < -500) {
                    interact.kb = interact_dep::kb_state::RC_ENABLE;
                } else if (rc.wheel > 500) {
                    interact.kb = interact_dep::kb_state::DISABLE;
                }
                if (interact.kb == interact_dep::kb_state::DISABLE) {
                    if (rc.left == static_cast<uint8_t>(lever::upper)
                        && rc.right == static_cast<uint8_t>(lever::upper)) {
                        interact.robo_arm.mode = interact_dep::robo_mode::DRAW;
                        interact.chassis.mode  = interact_dep::chassis_mode::CLIMB;
                    } else if (rc.left == static_cast<uint8_t>(lever::upper)
                               && rc.right == static_cast<uint8_t>(lever::middle)) {
                        interact.robo_arm.mode = interact_dep::robo_mode::RESET;
                        interact.chassis.mode  = interact_dep::chassis_mode::CLIMB;
                    } else if (rc.left == static_cast<uint8_t>(lever::middle)
                               && rc.right == static_cast<uint8_t>(lever::upper)) {
                        interact.robo_arm.mode = interact_dep::robo_mode::DRAW;
                        interact.chassis.mode  = interact_dep::chassis_mode::NORMAL;
                    } else if (rc.left == static_cast<uint8_t>(lever::middle)
                               && rc.right == static_cast<uint8_t>(lever::middle)) {
                        interact.robo_arm.mode = interact_dep::robo_mode::NONE;
                        interact.chassis.mode  = interact_dep::chassis_mode::NORMAL;
                    }
                }
            } else if (rc.left == static_cast<uint8_t>(lever::lower)) {
                if (interact.kb == interact_dep::kb_state::DISABLE) {
                    interact.chassis.mode = interact_dep::chassis_mode::NONE;
                    if (rc.right == static_cast<uint8_t>(lever::upper)) {
                        interact.robo_arm.mode = interact_dep::robo_mode::XYZ;
                    } else if (rc.right == static_cast<uint8_t>(lever::middle)) {
                        if (interact.robo_arm.mode != interact_dep::robo_mode::NORMAL1
                            && interact.robo_arm.mode != interact_dep::robo_mode::NORMAL2) {
                            interact.robo_arm.mode = interact_dep::robo_mode::NORMAL1;
                        }
                        if (rc.wheel < -500) {
                            interact.robo_arm.mode = interact_dep::robo_mode::NORMAL1;
                        } else if (rc.wheel > 500) {
                            interact.robo_arm.mode = interact_dep::robo_mode::NORMAL2;
                        }
                    }
                }
            } else if (rc.right == static_cast<uint8_t>(lever::lower)) {
                if (interact.kb == interact_dep::kb_state::DISABLE) {
                    interact.chassis.mode = interact_dep::chassis_mode::NONE;
                    if (interact.robo_arm.mode != interact_dep::robo_mode::CUSTOM
                        && interact.robo_arm.mode != interact_dep::robo_mode::VISION) {
                        interact.robo_arm.mode = interact_dep::robo_mode::VISION;
                    }
                    if (rc.wheel < -500) {
                        interact.robo_arm.mode = interact_dep::robo_mode::VISION;
                    } else if (rc.wheel > 500) {
                        interact.robo_arm.mode = interact_dep::robo_mode::CUSTOM;
                    }
                }
            }

            // 判断了kb    更新chassis的目标
            interact.update_chassis(chassis);
            // 还无判断kb
            interact.update_roboArm(roboArm);
        }
        interact.chassis.last_mode  = interact.chassis.mode;
        interact.robo_arm.last_mode = interact.robo_arm.mode;
        RemoteCtrlHeapCnt           = uxTaskGetStackHighWaterMark(NULL);
        osDelayUntil(&now, 14);
    }
}