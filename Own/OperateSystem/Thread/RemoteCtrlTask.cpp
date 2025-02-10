//
// Created by liaohy on 24-11-15.
//

#include <RemoteControl/RemoteControl.h>
#include "CppTask.h"
#include "Interact/Interact.h"
#include "ThreadConfig.h"
#include "MicroTime/MicroTime.h"
#include "Chassis/Chassis.h"

#ifdef __cplusplus
extern "C" {
#endif

extern osThreadId ERROR_TASKHandle;

#ifdef __cplusplus
}
#endif

MicroTime kineTime;

void RemoteCtrlTask() {
    using namespace my_math;
    using namespace roboarm_dep;
    while (1) {
        auto now = osKernelSysTick();
        if (!interact.remote_control.detect.isLost) {
            if (interact.remote_control.rcInfo.right == 2)
                osThreadResume(ERROR_TASKHandle);

            if (interact.remote_control.rcInfo.wheel > 500) {
                interact.remote_control.status = remote_ctrl_dep::status::KEYBOARD;
                interact.key_board = interact_dep::key_board::RC_ENABLE;
            } else if (interact.remote_control.rcInfo.wheel < -500) {
                interact.remote_control.status = remote_ctrl_dep::status::NORMAL;
                interact.key_board = interact_dep::key_board::DISABLE;
            }
            if (interact.remote_control.status == remote_ctrl_dep::status::NORMAL) {
                switch (interact.remote_control.rcInfo.right) {
                    case 1:
                        switch (interact.remote_control.rcInfo.left) {
                            case 1:

                                interact.chassis.mode = interact_dep::chassis_mode::NONE;
                                interact.robo_arm.mode = interact_dep::robo_mode::NORMAL;
                                break;
                            case 3:

                                interact.chassis.mode = interact_dep::chassis_mode::NONE;
                                interact.robo_arm.mode = interact_dep::robo_mode::NORMAL;
                                break;
                            case 2:

                                interact.chassis.mode = interact_dep::chassis_mode::NONE;
                                interact.robo_arm.mode = interact_dep::robo_mode::RESET;
                                break;
                            default:
                                break;
                        }
                        break;
                    case 3:
                        switch (interact.remote_control.rcInfo.left) {
                            case 1:

                                interact.chassis.mode = interact_dep::chassis_mode::NORMAL;
                                interact.robo_arm.mode = interact_dep::robo_mode::XYZ;
                                break;
                            case 3:


                                interact.chassis.mode = interact_dep::chassis_mode::ALL;
                                interact.robo_arm.mode = interact_dep::robo_mode::NONE;
                                break;
                            case 2:

                                interact.chassis.mode = interact_dep::chassis_mode::NONE;
                                interact.robo_arm.mode = interact_dep::robo_mode::VISION;
                                break;
                            default:
                                break;
                        }
                        break;
                    case 2:
                        osThreadResume(ERROR_TASKHandle);
                        break;
                    default:
                        break;

                }
            }

            xEventGroupSetBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT);
            xEventGroupWaitBits(osEventGroup, LK_RELETIVE_GET, pdFALSE, pdTRUE, portMAX_DELAY);

            interact.update_chassis(chassis);
            interact.update_roboArm(roboArm);

        } else {

            interact.chassis.mode = interact_dep::chassis_mode::NONE;
            interact.robo_arm.mode = interact_dep::robo_mode::NONE;
        }


        interact.chassis.last_mode = interact.chassis.mode;
        interact.robo_arm.last_mode = interact.robo_arm.mode;

        RemoteCtrlHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelayUntil(&now, 14);
    }
}