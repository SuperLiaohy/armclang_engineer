//
// Created by liaohy on 24-11-15.
//

#include <RemoteControl/RemoteControl.h>
#include "CppTask.h"
#include "RemoteControl/rcDependence.h"
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
                remote_control.ctrl.set_key_board();
            else if (remote_control.rcInfo.wheel < -500)
                remote_control.ctrl.set_normal();

            if (remote_control.ctrl.state.normal || remote_control.ctrl.state.vision || remote_control.ctrl.state.xyz || remote_control.ctrl.state.reset) {
                if (remote_control.rcInfo.right == 1 && remote_control.rcInfo.left == 1) {
                    remote_control.ctrl.set_vision();
                } else if (remote_control.rcInfo.right == 1 && remote_control.rcInfo.left == 3) {
                    remote_control.ctrl.set_xyz();
                } else if (remote_control.rcInfo.right == 1 && remote_control.rcInfo.left == 2) {
                    remote_control.ctrl.set_reset();
                } else {
                    remote_control.ctrl.set_normal();
                }
            }


            xEventGroupSetBits(osEventGroup, REMOTE_CONTROL_RECEIVE_EVENT);
            xEventGroupWaitBits(osEventGroup, LK_RELETIVE_GET, pdFALSE, pdTRUE, portMAX_DELAY);

            chassis.update_state(roboArm.real_relative_pos.joint1);

            if (remote_control.ctrl.state.xyz) {
                kineTime.start();
                if (interact.interaction != Interact::INTERACTION::REMOTE_CTRL_XYZ) {
                    interact.interaction = Interact::INTERACTION::REMOTE_CTRL_XYZ;
                    roboArm.fkine();
                    interact.pos[0] = roboArm.pos[0];
                    interact.pos[1] = roboArm.pos[1];
                    interact.pos[2] = roboArm.pos[2];
                }
                interact.receive_xyz(roboArm);
                kineTime.end();
            } else if (remote_control.ctrl.state.normal) {
                if (interact.interaction != Interact::INTERACTION::REMOTE_CTRL) {
                    interact.interaction = Interact::INTERACTION::REMOTE_CTRL;

                    interact.receive_data.joint1.angle = roboArm.real_relative_pos.joint1 * d2b2;
                    interact.receive_data.joint2.angle = roboArm.real_relative_pos.joint2 * d2b2;
                    interact.receive_data.joint3.angle = roboArm.real_relative_pos.joint3 * d2b2;
                    interact.receive_data.joint4.angle = roboArm.real_relative_pos.joint4 * d2b2;
                    //                    interact.receive_data.joint5.angle = ;
                }
                interact.receive_rc(roboArm);
            } else if (remote_control.ctrl.state.reset) {
                interact.interaction = Interact::INTERACTION::REMOTE_CTRL_RESET;

                interact.receive_data.joint4.angle = joint_scale(0, 360, 65536);
                interact.receive_data.joint3.angle = joint_scale(135, 360, 65536);
                interact.receive_data.joint2.angle = joint_scale(-80, 360, 65536);
                interact.receive_data.joint1.angle = joint_scale(0, 360, 65536);
                interact.receive_data.joint6.angle = joint_scale(0, 360, 8192);
                interact.receive_data.joint5.angle = joint_scale(-90, 360, 8192);

                roboArm.target.joint4.angle = roboArm.offset.joint4 * 100;
                roboArm.target.joint3.angle = (roboArm.offset.joint3 - 135) * 100;
                roboArm.target.joint2.angle = (roboArm.offset.joint2 - 80) * 100;
                roboArm.target.joint1.angle = roboArm.offset.joint1 * 100;
                xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
                roboArm.diff.init();
                xSemaphoreGive(CAN1MutexHandle);
            }

            RemoteCtrlHeapCnt = uxTaskGetStackHighWaterMark(NULL);
        }
        osDelayUntil(&now, 14);
    }
}