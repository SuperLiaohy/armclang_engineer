//
// Created by liaohy on 24-12-11.
//

#include "Interact.h"
#include "MyMath/MyMath.h"
#include "RoboArm/RoboArm.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "cmsis_os.h"
#include "FreeRTOS.h"

extern osMutexId CAN1MutexHandle;

#ifdef __cplusplus
}
#endif

void Interact::receive_cdc(uint8_t *data) {
    using namespace interact_dep;
//    if (interaction == VISION) {
//        if (data[0] == head && data[sizeof(receive_data_t) - 1] == tail) {
//            memcpy(&receive_data, data, sizeof(receive_data_t));
//        }
//    }
    if (robo_mode == robo_mode::VISION) {
        if (data[0] == head && data[sizeof(receive_data_t) - 1] == tail) {
            memcpy(&receive_data, data, sizeof(receive_data_t));
        }
    }

}

void Interact::receive_rc() {
    using namespace interact_dep;
    using namespace roboarm_dep;
    using namespace remote_ctrl_dep;
    using namespace my_math;
//    if (interaction == REMOTE_CTRL) {
//        if (remote_control.rcInfo.left == 1 && remote_control.rcInfo.right == 1) {
//            receive_data.joint4.angle =
//                    limited<int32_t>(static_cast<int32_t>(receive_data.joint4.angle +
//                                                          addSpeed(remote_control.rcInfo.ch1, 0.005) *
//                                                          limitation.joint4.max),
//                                     limitation.joint4.min, limitation.joint4.max);
//
////            Arm.target.joint4.angle = (receive_data.joint4.angle * b22d + Arm.offset.joint4) * scale(360, 36000);
//
//            receive_data.joint3.angle =
//                    limited<int32_t>(static_cast<int32_t>(receive_data.joint3.angle -
//                                                          addSpeed(remote_control.rcInfo.ch2, 0.005) *
//                                                          limitation.joint3.max),
//                                     limitation.joint3.min, limitation.joint3.max);
//
////            Arm.target.joint3.angle = (-receive_data.joint3.angle * b22d + Arm.offset.joint3) *
//            scale(360, 36000); //joint3是左手系 所以将右手系的数据取反
//
//            receive_data.joint2.angle =
//                    limited<int32_t>(static_cast<int32_t>(receive_data.joint2.angle +
//                                                          addSpeed(remote_control.rcInfo.ch4, 0.005) *
//                                                          limitation.joint2.max),
//                                     limitation.joint2.min, limitation.joint2.max);
//
////            Arm.target.joint2.angle = (receive_data.joint2.angle * b22d + Arm.offset.joint2) * scale(360, 36000);
//
//            receive_data.joint1.angle =
//                    limited<int32_t>(static_cast<int32_t>(receive_data.joint1.angle +
//                                                          addSpeed(remote_control.rcInfo.ch3, 0.01) *
//                                                          limitation.joint1.max),
//                                     limitation.joint1.min, limitation.joint1.max);
//
////            Arm.target.joint1.angle = (receive_data.joint1.angle * b22d + Arm.offset.joint1) * scale(360, 36000);
//        } else if (remote_control.rcInfo.left == 3 && remote_control.rcInfo.right == 1) {
//            // yaw_base pitch_1
//            receive_data.joint5.angle = limited<int32_t>(
//                    receive_data.joint5.angle + addSpeed(remote_control.rcInfo.ch2, 0.01) * limitation.joint5.max,
//                    limitation.joint5.min, limitation.joint5.max);
//            //                 receive_data.link6.angle = limited<int32_t>(receive_data.link6.angle + remoteControl->rcInfo.ch1 / 660.f * limition.link6.max / 50, -limition.link6.max, limition.link6.max);
//            receive_data.joint6.angle = addSpeed(remote_control.rcInfo.ch1, 0.01) * 8192;
//            totalRoll += receive_data.joint6.angle;
//            //                pitch =
//            //                roll =
//            //                roll = (5 + 6) / 2; pitch = (5 - 6) / 2
////            Arm.target.joint5.angle = scale_transmit<int64_t, float>(totalRoll + receive_data.joint5.angle, 4096, 180);
////            Arm.target.joint6.angle = scale_transmit<int64_t, float>(totalRoll - receive_data.joint5.angle, 4096, 180);
//            //                interact.receive_data.link1.angle = limited<uint16_t>(interact.receive_data.link1.angle + remote_control.rcInfo.ch1 / 660.f, -limition.link2.max, limition.link1.max);
//            //                interact.receive_data.link2.angle = limited<uint16_t>(interact.receive_data.link2.angle + remote_control.rcInfo.ch2 / 660.f, -limition.link2.max, limition.link2.max);
//        }
//    }
    if (robo_mode == robo_mode::NORMAL) {
        if (remote_control.rcInfo.left == 1 && remote_control.rcInfo.right == 1) {
            receive_data.joint4.angle =
                    limited<int32_t>(static_cast<int32_t>(receive_data.joint4.angle +
                                                          addSpeed(remote_control.rcInfo.ch1, 0.005) *
                                                          limitation.joint4.max),
                                     limitation.joint4.min, limitation.joint4.max);

//            Arm.target.joint4.angle = (receive_data.joint4.angle * b22d + Arm.offset.joint4) * scale(360, 36000);

            receive_data.joint3.angle =
                    limited<int32_t>(static_cast<int32_t>(receive_data.joint3.angle -
                                                          addSpeed(remote_control.rcInfo.ch2, 0.005) *
                                                          limitation.joint3.max),
                                     limitation.joint3.min, limitation.joint3.max);

//            Arm.target.joint3.angle = (-receive_data.joint3.angle * b22d + Arm.offset.joint3) *
            scale(360, 36000); //joint3是左手系 所以将右手系的数据取反

            receive_data.joint2.angle =
                    limited<int32_t>(static_cast<int32_t>(receive_data.joint2.angle +
                                                          addSpeed(remote_control.rcInfo.ch4, 0.005) *
                                                          limitation.joint2.max),
                                     limitation.joint2.min, limitation.joint2.max);

//            Arm.target.joint2.angle = (receive_data.joint2.angle * b22d + Arm.offset.joint2) * scale(360, 36000);

            receive_data.joint1.angle =
                    limited<int32_t>(static_cast<int32_t>(receive_data.joint1.angle +
                                                          addSpeed(remote_control.rcInfo.ch3, 0.01) *
                                                          limitation.joint1.max),
                                     limitation.joint1.min, limitation.joint1.max);

//            Arm.target.joint1.angle = (receive_data.joint1.angle * b22d + Arm.offset.joint1) * scale(360, 36000);
        } else if (remote_control.rcInfo.left == 3 && remote_control.rcInfo.right == 1) {
            // yaw_base pitch_1
            receive_data.joint5.angle = limited<int32_t>(
                    receive_data.joint5.angle + addSpeed(remote_control.rcInfo.ch2, 0.01) * limitation.joint5.max,
                    limitation.joint5.min, limitation.joint5.max);
            //                 receive_data.link6.angle = limited<int32_t>(receive_data.link6.angle + remoteControl->rcInfo.ch1 / 660.f * limition.link6.max / 50, -limition.link6.max, limition.link6.max);
            receive_data.joint6.angle = addSpeed(remote_control.rcInfo.ch1, 0.01) * 8192;
            totalRoll += receive_data.joint6.angle;
            //                pitch =
            //                roll =
            //                roll = (5 + 6) / 2; pitch = (5 - 6) / 2
//            Arm.target.joint5.angle = scale_transmit<int64_t, float>(totalRoll + receive_data.joint5.angle, 4096, 180);
//            Arm.target.joint6.angle = scale_transmit<int64_t, float>(totalRoll - receive_data.joint5.angle, 4096, 180);
            //                interact.receive_data.link1.angle = limited<uint16_t>(interact.receive_data.link1.angle + remote_control.rcInfo.ch1 / 660.f, -limition.link2.max, limition.link1.max);
            //                interact.receive_data.link2.angle = limited<uint16_t>(interact.receive_data.link2.angle + remote_control.rcInfo.ch2 / 660.f, -limition.link2.max, limition.link2.max);
        }
    }
}

void Interact::receive_xyz(RoboArm &Arm) {
    using namespace interact_dep;
    using namespace remote_ctrl_dep;
    using namespace roboarm_dep;
    using namespace my_math;
//    if (interaction == REMOTE_CTRL_XYZ) {
//        pos[0] += addSpeed(remote_control.rcInfo.ch1, 1);
//        pos[2] += addSpeed(remote_control.rcInfo.ch2, 1);
//
//        Arm.fkine();
//        if (Arm.ikine(interact.pos)) {
//            pos[0] = Arm.pos[0];
//            pos[1] = Arm.pos[1];
//            pos[2] = Arm.pos[2];
//        }
//
//        limited<float>(Arm.q[0], limitation.joint1.min * b22d, limitation.joint1.max * d2b2);
//        limited<float>(Arm.q[1], limitation.joint2.min * b22d, limitation.joint2.max * d2b2);
//        limited<float>(Arm.q[2], limitation.joint3.min * b22d, limitation.joint3.max * d2b2);
//
////        Arm.target.joint3.angle = (-Arm.q[2] + Arm.offset.joint3) * scale(360, 36000); //joint3是左手系 所以将右手系的数据取反
//
////        Arm.target.joint2.angle = (Arm.q[1] + Arm.offset.joint2) * scale(360, 36000);
//
////        Arm.target.joint1.angle = (Arm.q[0] + Arm.offset.joint1) * scale(360, 36000);
//    }
    if (robo_mode == robo_mode::XYZ) {
        pos[0] += addSpeed(remote_control.rcInfo.ch1, 1);
        pos[2] += addSpeed(remote_control.rcInfo.ch2, 1);

        Arm.fkine();
        if (Arm.ikine(interact.pos)) {
            pos[0] = Arm.pos[0];
            pos[1] = Arm.pos[1];
            pos[2] = Arm.pos[2];
        }

        limited<float>(Arm.q[0], limitation.joint1.min * b22d, limitation.joint1.max * d2b2);
        limited<float>(Arm.q[1], limitation.joint2.min * b22d, limitation.joint2.max * d2b2);
        limited<float>(Arm.q[2], limitation.joint3.min * b22d, limitation.joint3.max * d2b2);

//        Arm.target.joint3.angle = (-Arm.q[2] + Arm.offset.joint3) * scale(360, 36000); //joint3是左手系 所以将右手系的数据取反

//        Arm.target.joint2.angle = (Arm.q[1] + Arm.offset.joint2) * scale(360, 36000);

//        Arm.target.joint1.angle = (Arm.q[0] + Arm.offset.joint1) * scale(360, 36000);
    }
}

void Interact::transmit_relative_pos(RoboArm &Arm) {
    using namespace my_math;
    transmit_data.joint1.angle = Arm.real_relative_pos.joint1 * d2b2;
    transmit_data.joint2.angle = Arm.real_relative_pos.joint2 * d2b2;
    transmit_data.joint3.angle = Arm.real_relative_pos.joint3 * d2b2;
    transmit_data.joint4.angle = Arm.real_relative_pos.joint4 * d2b2;
    transmit_data.joint5.angle = 0;
    transmit_data.joint6.angle = 0;
    transmit();
}

void Interact::receive_reset(RoboArm &Arm) {
    using namespace roboarm_dep;
    receive_data.joint4.angle = joint_scale(0, 360, 65536);
    receive_data.joint3.angle = joint_scale(135, 360, 65536);
    receive_data.joint2.angle = joint_scale(-80, 360, 65536);
    receive_data.joint1.angle = joint_scale(0, 360, 65536);
    receive_data.joint6.angle = joint_scale(0, 360, 8192);
    receive_data.joint5.angle = joint_scale(-90, 360, 8192);

//    Arm.target.joint4.angle = Arm.offset.joint4 * 100;
//    Arm.target.joint3.angle = (Arm.offset.joint3 - 135) * 100;
//    Arm.target.joint2.angle = (Arm.offset.joint2 - 80) * 100;
//    Arm.target.joint1.angle = Arm.offset.joint1 * 100;
    xSemaphoreTake(CAN1MutexHandle, portMAX_DELAY);
    Arm.diff.init();
    xSemaphoreGive(CAN1MutexHandle);
}

void Interact::receive_custom(uint8_t *data) {
    using namespace interact_dep;
//    if (interaction == CUSTOM) {
//        memcpy(reinterpret_cast<uint8_t *>(&receive_data) + sizeof(receive_data.head), data,
//               sizeof(receive_data_t) - sizeof(receive_data.head) - sizeof(receive_data.tail));
//    }
    if (robo_mode == robo_mode::CUSTOM) {
        memcpy(reinterpret_cast<uint8_t *>(&receive_data) + sizeof(receive_data.head), data,
               sizeof(receive_data_t) - sizeof(receive_data.head) - sizeof(receive_data.tail));
    }

}


void remote_ctrl_recover() {
    //    remote_control.clear();
    buzzer.StartMusic(error_music, 2);
    interact.remote_control.start();
}

