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
                                chassis.mode = chassis_dep::mode::NONE;
//                                interact.interaction = interact_dep::INTERACTION::REMOTE_CTRL;

                                interact.chassis_mode = interact_dep::chassis_mode::NONE;
                                interact.robo_mode = interact_dep::robo_mode::NORMAL;
                                break;
                            case 3:
                                chassis.mode = chassis_dep::mode::NONE;
//                                interact.interaction = interact_dep::INTERACTION::REMOTE_CTRL;

                                interact.chassis_mode = interact_dep::chassis_mode::NONE;
                                interact.robo_mode = interact_dep::robo_mode::NORMAL;
                                break;
                            case 2:
                                chassis.mode = chassis_dep::mode::NONE;
//                                interact.interaction = interact_dep::INTERACTION::REMOTE_CTRL_RESET;

                                interact.chassis_mode = interact_dep::chassis_mode::NONE;
                                interact.robo_mode = interact_dep::robo_mode::RESET;
                                break;
                            default:
                                break;
                        }
                        break;
                    case 3:
                        switch (interact.remote_control.rcInfo.left) {
                            case 1:
                                chassis.mode = chassis_dep::mode::Work;
//                                interact.interaction = interact_dep::INTERACTION::REMOTE_CTRL_XYZ;

                                interact.chassis_mode = interact_dep::chassis_mode::NORMAL;
                                interact.robo_mode = interact_dep::robo_mode::XYZ;
                                break;
                            case 3:
                                chassis.mode = chassis_dep::mode::Follow;
//                                interact.interaction = interact_dep::INTERACTION::NONE;

                                interact.chassis_mode = interact_dep::chassis_mode::ALL;
                                interact.robo_mode = interact_dep::robo_mode::NONE;
                                break;
                            case 2:
                                chassis.mode = chassis_dep::mode::NONE;
//                                interact.interaction = interact_dep::INTERACTION::VISION;

                                interact.chassis_mode = interact_dep::chassis_mode::NONE;
                                interact.robo_mode = interact_dep::robo_mode::VISION;
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

            chassis.update_state(interact.remote_control, roboArm.real_relative_pos.joint1);

//            switch (interact.interaction) {
//                case interact_dep::INTERACTION::REMOTE_CTRL:
//                    if (interact.last_interaction != interact_dep::INTERACTION::REMOTE_CTRL) {
//                        interact.receive_data.joint1.angle = roboArm.real_relative_pos.joint1 * d2b2;
//                        interact.receive_data.joint2.angle = roboArm.real_relative_pos.joint2 * d2b2;
//                        interact.receive_data.joint3.angle = roboArm.real_relative_pos.joint3 * d2b2;
//                        interact.receive_data.joint4.angle = roboArm.real_relative_pos.joint4 * d2b2;
//
////                        interact.receive_data.joint5.angle = roboArm.real_relative_pos.joint5 * d2b2;
//                    }
//                    interact.receive_rc();
//                    break;
//                case interact_dep::INTERACTION::REMOTE_CTRL_XYZ:
//                    kineTime.start();
//                    if (interact.last_interaction != interact_dep::INTERACTION::REMOTE_CTRL_XYZ) {
//                        roboArm.fkine();
//                        interact.pos[0] = roboArm.pos[0];
//                        interact.pos[1] = roboArm.pos[1];
//                        interact.pos[2] = roboArm.pos[2];
//                    }
//                    interact.receive_xyz(roboArm);
//                    kineTime.end();
//                    break;
//                case interact_dep::INTERACTION::REMOTE_CTRL_RESET:
//                    interact.receive_reset(roboArm);
//                    break;
//                case interact_dep::INTERACTION::VISION:
//                    break;
//                default:
//                    break;
//            }
            switch (interact.robo_mode) {
                case interact_dep::robo_mode::NORMAL:
                    if (interact.last_robo_mode != interact_dep::robo_mode::NORMAL) {
                        interact.receive_data.joint1.angle = roboArm.real_relative_pos.joint1 * d2b2;
                        interact.receive_data.joint2.angle = roboArm.real_relative_pos.joint2 * d2b2;
                        interact.receive_data.joint3.angle = roboArm.real_relative_pos.joint3 * d2b2;
                        interact.receive_data.joint4.angle = roboArm.real_relative_pos.joint4 * d2b2;

//                        interact.receive_data.joint5.angle = roboArm.real_relative_pos.joint5 * d2b2;
                    }
                    interact.receive_rc();
                    break;
                case interact_dep::robo_mode::XYZ:
                    kineTime.start();
                    if (interact.last_robo_mode != interact_dep::robo_mode::XYZ) {
                        roboArm.fkine();
                        interact.pos[0] = roboArm.pos[0];
                        interact.pos[1] = roboArm.pos[1];
                        interact.pos[2] = roboArm.pos[2];
                    }
                    interact.receive_xyz(roboArm);
                    kineTime.end();
                    break;
                case interact_dep::robo_mode::RESET:
                    interact.receive_reset(roboArm);
                    break;
                case interact_dep::robo_mode::VISION:
                    break;
                default:
                    break;
            }
        } else {
            chassis.mode = chassis_dep::mode::NONE;
//            interact.interaction = interact_dep::INTERACTION::NONE;

            interact.chassis_mode = interact_dep::chassis_mode::NONE;
            interact.robo_mode = interact_dep::robo_mode::NONE;
        }

        chassis.last_mode = chassis.mode;
//        interact.last_interaction = interact.interaction;

        interact.last_chassis_mode = interact.chassis_mode;
        interact.last_robo_mode = interact.robo_mode;

        RemoteCtrlHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelayUntil(&now, 14);
    }
}