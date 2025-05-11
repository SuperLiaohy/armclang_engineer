//
// Created by liaohy on 24-12-11.
//

#include "Interact.hpp"
#include "Chassis/Chassis.hpp"
#include "MyMath/MyMath.hpp"
#include "RoboArm/RoboArm.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "cmsis_os.h"

extern osMutexId CAN1MutexHandle;

#ifdef __cplusplus
}
#endif

void Interact::receive_cdc(uint8_t* data) {
    using namespace interact_dep;
    if (robo_arm.mode == robo_mode::VISION) {
        if (data[0] == pc.head && data[sizeof(pc_dep::rx_frame) - 1] == pc.tail) {
            memcpy(&pc.rx_frame, data, sizeof(pc_dep::rx_frame));
            joint[0] = pc.rx_frame.data.joint1.angle * scale(65536, 360);
            joint[1] = pc.rx_frame.data.joint2.angle * scale(65536, 360);
            joint[2] = pc.rx_frame.data.joint3.angle * scale(65536, 360);
            joint[3] = pc.rx_frame.data.joint4.angle * scale(65536, 360);
            joint[4] = pc.rx_frame.data.joint5.angle * scale(8192, 360);
            joint[5] = pc.rx_frame.data.joint6.angle * scale(8192, 360);
        }
    }
}

void Interact::receive_rc() {
    using namespace interact_dep;
    using namespace roboarm_dep;
    using namespace remote_ctrl_dep;
    using namespace my_math;

    if (robo_arm.mode == robo_mode::NORMAL1) {
        joint[3] = joint[3] - addSpeed(remote_control.rcInfo.ch1, 0.005) * limitation.joint4.max;
        joint[2] = limited<float>(joint[2] - addSpeed(remote_control.rcInfo.ch2, 0.005) * limitation.joint3.max,
                                  limitation.joint3.min, limitation.joint3.max);
        joint[1] = limited<float>(joint[1] + addSpeed(remote_control.rcInfo.ch4, 0.01) * limitation.joint2.max,
                                  limitation.joint2.min, limitation.joint2.max);
        joint[0] = limited<float>(joint[0] + addSpeed(remote_control.rcInfo.ch3, 0.01) * limitation.joint1.max,
                                  limitation.joint1.min, limitation.joint1.max);
    } else if (robo_arm.mode == robo_mode::NORMAL2) {
        // pitch
        joint[4] = limited<float>(joint[4] + addSpeed(remote_control.rcInfo.ch2, 0.01) * limitation.joint5.max,
                                  limitation.joint5.min, limitation.joint5.max);
        // yaw
        joint[5] = joint[5] + addSpeed(remote_control.rcInfo.ch1, 0.01) * limitation.joint6.max;
    }
}

void Interact::receive_xyz(RoboArm& Arm) {
    using namespace interact_dep;
    using namespace remote_ctrl_dep;
    using namespace roboarm_dep;
    using namespace my_math;
    if (robo_arm.mode == robo_mode::XYZ) {
        remote_control.pos[0] += addSpeed(remote_control.rcInfo.ch1, 2.5);
        remote_control.pos[2] += addSpeed(remote_control.rcInfo.ch2, 2.5);
        if (!Arm.ikine(remote_control.pos)) {
            Arm.fkine(remote_control.pos);
        } else {
            joint[0] = limited<float>(Arm.q[0], limitation.joint1.min, limitation.joint1.max);
            joint[1] = limited<float>(Arm.q[1], limitation.joint2.min, limitation.joint2.max);
            joint[2] = limited<float>(Arm.q[2], limitation.joint3.min, limitation.joint3.max);
            joint[4] = limited<float>(180 - (joint[1] + joint[2]), limitation.joint5.min, limitation.joint5.max);
        }
    }
}

void Interact::transmit_relative_pos(const std::array<float, 6>& pos) {
    using namespace my_math;
    pc.tx_frame.data.joint1.angle = pos[0] * d2b2;
    pc.tx_frame.data.joint2.angle = pos[1] * d2b2;
    pc.tx_frame.data.joint3.angle = pos[2] * d2b2;
    pc.tx_frame.data.joint4.angle = pos[3] * d2b2;
    pc.tx_frame.data.joint5.angle = pos[4] * scale(360, 8192);
    pc.tx_frame.data.joint6.angle = static_cast<int>((pos[5] * scale(360, 8192))) % 8192;
    pc.transmit(reinterpret_cast<uint8_t*>(&pc.tx_frame), sizeof(pc.tx_frame));
}

void Interact::receive_reset() {
    joint[0] = 0;
    joint[1] = -55;
    joint[2] = 145;
    joint[3] = 0;
    joint[4] = 0;
    joint[5] = 0;
}
void air_left_callback(KeyEventType event);
void air_right_callback(KeyEventType event);
void Interact::receive_custom(uint8_t* data) {
    using namespace interact_dep;
    if (robo_arm.mode == robo_mode::CUSTOM) {
        auto last_s = image_trans_dep::user_custom_rx_status(image_trans.user_custom_rx_data.s);
        memcpy(reinterpret_cast<uint8_t*>(&image_trans.user_custom_rx_data), data,
               sizeof(image_trans_dep::user_custom_rx_data));
        if (image_trans.user_custom_rx_data.s.pump != last_s.pump) {
            air_right_callback(KeyEvent_OnClick);
        }
        if (image_trans.user_custom_rx_data.s.valve != last_s.valve) {
            air_left_callback(KeyEvent_OnClick);
        }
        if (!image_trans.read_map_back()) {
            joint[0] = -image_trans.user_custom_rx_data.joint[0] * scale(4096, 360);
            joint[1] = image_trans.user_custom_rx_data.joint[1] * scale(4096, 360);
            joint[2] = image_trans.user_custom_rx_data.joint[2] * scale(4096, 360);
            joint[3] = -image_trans.user_custom_rx_data.joint[3] * scale(4096, 360);
            joint[4] = image_trans.user_custom_rx_data.joint[4] * scale(4096, 360);
            joint[5] = -image_trans.user_custom_rx_data.joint[5] * scale(4096, 360);
        }
    }
}

void Interact::update_roboArm(RoboArm& Arm) {
    using namespace my_math;
    switch (robo_arm.mode) {
        case interact_dep::robo_mode::NORMAL1:
            if (robo_arm.last_mode != interact_dep::robo_mode::NORMAL1) {
                joint[0] = Arm.relative_pos[0];
                joint[1] = Arm.relative_pos[1];
                joint[2] = Arm.relative_pos[2];
                joint[3] = Arm.relative_pos[3];
            }
            receive_rc();
            break;
        case interact_dep::robo_mode::NORMAL2:
            if (robo_arm.last_mode != interact_dep::robo_mode::NORMAL2) {
                joint[0] = Arm.relative_pos[0];
                joint[1] = Arm.relative_pos[1];
                joint[2] = Arm.relative_pos[2];
                joint[3] = Arm.relative_pos[3];
            }
            receive_rc();
            break;
        case interact_dep::robo_mode::XYZ:
            if (robo_arm.last_mode != interact_dep::robo_mode::XYZ) { Arm.fkine(remote_control.pos); }
            receive_xyz(Arm);
            break;
        case interact_dep::robo_mode::RESET: receive_reset(); break;
        case interact_dep::robo_mode::ACTIONS: {
            interact.receive_actions(Arm);
            break;
        }
        default: break;
    }
}

void Interact::update_chassis(Chassis& cha) {
    using namespace chassis_dep;
    using namespace remote_ctrl_dep;
    using namespace my_math;
    switch (chassis.mode) {
        case interact_dep::chassis_mode::CLIMB:
            switch (kb) {
                case interact_dep::kb_state::RC_ENABLE:
                case interact_dep::kb_state::IM_ENABLE:
                    cha.move.xSlope.target_set(max.vx * static_cast<float>((cha.key.d + cha.key.a)));
                    cha.move.ySlope.target_set(max.vy * static_cast<float>((cha.key.w + cha.key.s)));
                    cha.move.wSlope.target_set(-max.w * static_cast<float>((interact.key_board.mouse.x)) / 32767 * 40);
                    break;
                case interact_dep::kb_state::DISABLE:
                    cha.move.xSlope.target_set(addSpeed(remote_control.rcInfo.ch3, max.vx));
                    cha.move.ySlope.target_set(addSpeed(remote_control.rcInfo.ch4, max.vy));
                    cha.move.extendSlope.target_set(addSpeed(remote_control.rcInfo.ch2, max.vy));
                    cha.move.wSlope.target_set(-addSpeed(remote_control.rcInfo.ch1, max.w));
                    break;
            }
            break;
        case interact_dep::chassis_mode::NORMAL:
            switch (kb) {
                case interact_dep::kb_state::RC_ENABLE:
                case interact_dep::kb_state::IM_ENABLE:
                    cha.move.xSlope.target_set(max.vx * static_cast<float>((cha.key.d + cha.key.a)));
                    cha.move.ySlope.target_set(max.vy * static_cast<float>((cha.key.w + cha.key.s)));
                    cha.move.wSlope.target_set(-max.w * static_cast<float>((interact.key_board.mouse.x)) / 32767 * 40);
                    break;
                case interact_dep::kb_state::DISABLE:
                    cha.move.xSlope.target_set(addSpeed(remote_control.rcInfo.ch3, max.vx));
                    cha.move.ySlope.target_set(addSpeed(remote_control.rcInfo.ch4, max.vy));
                    cha.move.extendSlope.target_set(addSpeed(remote_control.rcInfo.ch2, max.vy));
                    cha.move.wSlope.target_set(-addSpeed(remote_control.rcInfo.ch1, max.w));
                    break;
            }
            break;
        case interact_dep::chassis_mode::NONE:
            cha.move.xSlope.target_set(0);
            cha.move.ySlope.target_set(0);
            cha.move.wSlope.target_set(0);
            break;
        default: break;
    }
}
void Interact::receive_actions(RoboArm& Arm) {
    using namespace interact_dep;
    using namespace roboarm_dep;
    if (robo_arm.mode == robo_mode::ACTIONS) {
        switch (actions->status) {
            case interact_dep::action_status::Joints:
                joint[0] = actions->joints[0];
                joint[1] = actions->joints[1];
                joint[2] = actions->joints[2];
                joint[3] = actions->joints[3];
                joint[4] = actions->joints[4];
                joint[5] = actions->joints[5];
                break;
            case interact_dep::action_status::CartesianX: {
                if (actions->init == false) {
                    Arm.fkine(actions->pos);
                    actions->init = true;
                }
                actions->pos[0] = actions->axis_value.update();
                if (!Arm.ikine(actions->pos)) {
                    // Arm.fkine(pos);
                } else {
                    joint[0] = limited<float>(Arm.q[0], limitation.joint1.min, limitation.joint1.max);
                    joint[1] = limited<float>(Arm.q[1], limitation.joint2.min, limitation.joint2.max);
                    joint[2] = limited<float>(Arm.q[2], limitation.joint3.min, limitation.joint3.max);
                    joint[4] = limited<float>(90 - (joint[1] + joint[2]), limitation.joint5.min, limitation.joint5.max);
                }
            } break;
            case interact_dep::action_status::CartesianZ: {
                if (actions->init == false) {
                    Arm.fkine(actions->pos);
                    actions->axis_value.value_set(actions->pos[2]);
                    actions->init = true;
                }
                actions->pos[2] = actions->axis_value.update();
                if (!Arm.ikine(actions->pos)) {
                    // Arm.fkine(pos);
                } else {
                    joint[0] = limited<float>(Arm.q[0], limitation.joint1.min, limitation.joint1.max);
                    joint[1] = limited<float>(Arm.q[1], limitation.joint2.min, limitation.joint2.max);
                    joint[2] = limited<float>(Arm.q[2], limitation.joint3.min, limitation.joint3.max);
                    joint[4] =
                        limited<float>(180 - (joint[1] + joint[2]), limitation.joint5.min, limitation.joint5.max);
                }
            } break;
            default: break;
        }
    }
}

void remote_ctrl_recover() {
    buzzer.PushMusic<8>(Buzzer::error_music);
    interact.remote_control.start();
}
