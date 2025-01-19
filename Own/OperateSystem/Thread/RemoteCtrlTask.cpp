//
// Created by liaohy on 24-11-15.
//

#include <RemoteControl/RemoteControl.h>
#include "CppTask.h"
#include "RemoteControl/remote_ctrl_dep.h"
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
    using namespace robo_arm;

    while (1) {
        auto now = osKernelSysTick();
        if (!remote_control.detect.isLost) {
            if (remote_control.rcInfo.right == 2)
                osThreadResume(ERROR_TASKHandle);

            if (remote_control.rcInfo.wheel > 500)
                remote_control.status = remote_ctrl_dep::status::KEYBOARD;
            else if (remote_control.rcInfo.wheel < -500)
                remote_control.status = remote_ctrl_dep::status::NORMAL;

            if (remote_control.status == remote_ctrl_dep::status::NORMAL) {
                switch (remote_control.rcInfo.right) {
                    case 1:
                        switch (remote_control.rcInfo.left) {
                            case 1:
                                chassis.mode = chassis_dep::mode::NONE;
                                interact.interaction = Interact::INTERACTION::REMOTE_CTRL;
                                break;
                            case 3:
                                chassis.mode = chassis_dep::mode::NONE;
                                interact.interaction = Interact::INTERACTION::REMOTE_CTRL;
                                break;
                            case 2:
                                chassis.mode = chassis_dep::mode::NONE;
                                interact.interaction = Interact::INTERACTION::REMOTE_CTRL_RESET;
                                break;
                            default:
                                break;
                        }
                        break;
                    case 3:
                        switch (remote_control.rcInfo.left) {
                            case 1:
                                chassis.mode = chassis_dep::mode::Work;
                                interact.interaction = Interact::INTERACTION::REMOTE_CTRL_XYZ;
                                break;
                            case 3:
                                chassis.mode = chassis_dep::mode::Follow;
                                interact.interaction = Interact::INTERACTION::NONE;
                                break;
                            case 2:
                                chassis.mode = chassis_dep::mode::NONE;
                                interact.interaction = Interact::INTERACTION::VISION;
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

            chassis.update_state(roboArm.real_relative_pos.joint1);

            switch (interact.interaction) {
                case Interact::INTERACTION::REMOTE_CTRL:
                    if (interact.last_interaction != Interact::INTERACTION::REMOTE_CTRL) {
                        interact.receive_data.joint1.angle = roboArm.real_relative_pos.joint1 * d2b2;
                        interact.receive_data.joint2.angle = roboArm.real_relative_pos.joint2 * d2b2;
                        interact.receive_data.joint3.angle = roboArm.real_relative_pos.joint3 * d2b2;
                        interact.receive_data.joint4.angle = roboArm.real_relative_pos.joint4 * d2b2;

//                        interact.receive_data.joint5.angle = roboArm.real_relative_pos.joint5 * d2b2;
                    }
                    interact.receive_rc(roboArm);
                    break;
                case Interact::INTERACTION::REMOTE_CTRL_XYZ:
                    kineTime.start();
                    if (interact.last_interaction != Interact::INTERACTION::REMOTE_CTRL_XYZ) {
                        roboArm.fkine();
                        interact.pos[0] = roboArm.pos[0];
                        interact.pos[1] = roboArm.pos[1];
                        interact.pos[2] = roboArm.pos[2];
                    }
                    interact.receive_xyz(roboArm);
                    kineTime.end();
                    break;
                case Interact::INTERACTION::REMOTE_CTRL_RESET:
                    interact.receive_reset(roboArm);
                    break;
                case Interact::INTERACTION::VISION:
                    break;
                default:
                    break;
            }
        } else {
            chassis.mode = chassis_dep::mode::NONE;
            interact.interaction = Interact::INTERACTION::NONE;
        }

        chassis.last_mode = chassis.mode;
        interact.interaction = interact.last_interaction;

        RemoteCtrlHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        osDelayUntil(&now, 14);
    }
}