//
// Created by Administrator on 24-10-23.
//

#include "RoboArm.h"
#include "Interact/Interact.h"

inline float slove_q2(float q1_slove, float q3_slove, float x, float y, float z) {
    using namespace my_math;
    using namespace roboarm_dep;

    float TMP_A = A * arm_cos_f32(q3_slove) + B;
    float TMP_B = A * arm_sin_f32(q3_slove);
    float tmp_cos;
    float tmp_sin;

    if (is_equal(q1_slove, pi / 2) || is_equal(q1_slove, -pi / 2)) {
        tmp_cos = (y * TMP_B / arm_sin_f32(q1_slove) + z * TMP_A);
        tmp_sin = (y * TMP_A / arm_sin_f32(q1_slove) - z * TMP_B);
    } else {
        tmp_cos = (x * TMP_B / arm_cos_f32(q1_slove) + z * TMP_A);
        tmp_sin = (x * TMP_A / arm_cos_f32(q1_slove) - z * TMP_B);
    }
    return arm_atan2_f32(tmp_sin, tmp_cos);
};

float left_angle = 0;
float right_angle = 0;

void RoboArm::enable() {
    using namespace roboarm_dep;
    // 先关闭再开启
    // 关闭是为了清除多圈状态
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint1.motor.close_flag) {
            break;
        }
        joint1.motor.close();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint2.internal.motor.close_flag) {
            break;
        }
        joint2.internal.motor.close();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint2.external.motor.close_flag) {
            break;
        }
        joint2.external.motor.close();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint3.motor.close_flag) {
            break;
        }
        joint3.motor.close();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint4.motor.close_flag) {
            break;
        }
        joint4.motor.close();
        HAL_Delay(1);
    }



    HAL_Delay(1);

    // 以下为开启电机状态
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint1.motor.start_flag) {
            break;
        }
        joint1.motor.enable();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint2.internal.motor.start_flag) {
            break;
        }
        joint2.internal.motor.enable();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint2.external.motor.start_flag) {
            break;
        }
        joint2.external.motor.enable();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint3.motor.start_flag) {
            break;
        }
        joint3.motor.enable();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint4.motor.start_flag) {
            break;
        }
        joint4.motor.enable();
        HAL_Delay(1);
    }


}

void RoboArm::disable() {
    // 不会清除多圈状态
    joint1.motor.disable();
    joint2.internal.motor.disable();
    joint2.external.motor.disable();
    joint3.motor.disable();
    joint4.motor.disable();
}

void RoboArm::close() {
    // 会清除多圈状态
    joint1.motor.close();
    joint2.internal.motor.close();
    joint2.external.motor.close();
    joint3.motor.close();
    joint4.motor.close();
}

void RoboArm::init_offset(Interact &interaction) {
    using namespace roboarm_dep;
    joint4.motor.read_totalposition();
    joint3.motor.read_totalposition();
    joint2.internal.motor.read_totalposition();
    joint2.external.motor.read_totalposition();
    joint1.motor.read_totalposition();

    interaction.receive_data.joint6.angle = joint_scale(0, 360, 8192);
    interaction.receive_data.joint5.angle = joint_scale(-90, 360, 8192);

    target.joint6.angle = offset.joint6;
    target.joint5.angle = offset.joint5;
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint4.motor.offset_flag) {
            interaction.receive_data.joint4.angle = joint_scale(0, 360, 65536);
            if (joint4.feed_back.totalPosition < 0) { offset.joint4 -= 360; }
            target.joint4.angle = offset.joint4 * 100;
            break;
        }
        joint4.motor.read_totalposition();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint3.motor.offset_flag) {
            interaction.receive_data.joint3.angle = joint_scale(135, 360, 65536);
            if (joint3.feed_back.totalPosition < 0) { offset.joint3 -= 360; }
            target.joint3.angle = (offset.joint3 - 135) * 100;
            break;
        }
        joint3.motor.read_totalposition();
        HAL_Delay(1);
    }
//    for (uint32_t i = 0; i < MaxTimeOut; i++) {
//        if (joint2.motor.start_flag) {
//            interaction.receive_data.joint2.angle = joint_scale(-80, 360, 65536);
//            if (joint2.feed_back.totalPosition < 0) { offset.joint2 -= 360; }
//            target.joint2.angle = (offset.joint2 - 80) * 100;
//            break;
//        }
//        joint2.motor.read_totalposition();
//        HAL_Delay(1);
//    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint2.internal.motor.offset_flag) {
            interaction.receive_data.joint2.angle = joint_scale(-50, 360, 65536);
            if (joint2.internal.feed_back.totalPosition < 0) { offset.joint2.internal -= 360; }
            target.joint2.internal.angle = (offset.joint2.internal - 50) * 100;
            break;
        }
        joint2.internal.motor.read_totalposition();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint2.external.motor.offset_flag) {
            interaction.receive_data.joint2.angle = joint_scale(-50, 360, 65536);
            if (joint2.external.feed_back.totalPosition < 0) { offset.joint2.external -= 360; }
            target.joint2.external.angle = (offset.joint2.external - 50) * 100;
            break;
        }
        joint2.external.motor.read_totalposition();
        HAL_Delay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint1.motor.offset_flag) {
            interaction.receive_data.joint1.angle = joint_scale(0, 360, 65536);
            if (joint1.feed_back.totalPosition < 0) { offset.joint1 -= 360; }
            target.joint1.angle = offset.joint1 * 100;
            break;
        }
        joint1.motor.read_totalposition();
        HAL_Delay(1);
    }
}

void RoboArm::update_relative_pos() {
    //    real_relative_pos.joint1 = static_cast<int16_t>(scale_transmit<float, int32_t>(joint1.feed_back.Data.position - offset.joint1, 45, limition.joint1));
    //    real_relative_pos.joint1 = offset_transmit<float>(joint1.feed_back.Data.position - offset.joint1, 0, -180, 180);
    real_relative_pos.joint1 = joint1.feed_back.totalPosition - offset.joint1;

    //    real_relative_pos.joint2 = static_cast<int16_t>(scale_transmit<float, int32_t>(joint2.feed_back.Data.position - offset.joint2, 80, limition.joint2));
    //    real_relative_pos.joint2 = offset_transmit<float>(joint2.feed_back.Data.position - offset.joint2, 0, -180, 180);
//    real_relative_pos.joint2 = joint2.feed_back.totalPosition - offset.joint2;
    // 相对旋转角度只需要一个电机就可以确定，所以只需要一个电机的反馈数据，这里选择外侧电机
    real_relative_pos.joint2 = joint2.external.feed_back.totalPosition - offset.joint2.external;

    //    real_relative_pos.joint3 = static_cast<int16_t>(scale_transmit<float, int32_t>(- (joint3.feed_back.Data.position - offset.joint3), 135, limition.joint3));
    //    real_relative_pos.joint3 = -offset_transmit<float>((joint3.feed_back.Data.position - offset.joint3), 0, -180, 180);
    real_relative_pos.joint3 = -(joint3.feed_back.totalPosition - offset.joint3);

    //    real_relative_pos.joint4 = static_cast<int16_t>(scale_transmit<float, int32_t>(joint4.feed_back.Data.position - offset.joint4, 180, limition.joint4));
    //    real_relative_pos.joint4 = offset_transmit<float>(joint4.feed_back.Data.position - offset.joint4, 0, -180, 180);
    real_relative_pos.joint4 = joint4.feed_back.totalPosition - offset.joint4;

    diff.update_relative_pos(real_relative_pos.joint5, real_relative_pos.joint6);
}

void RoboArm::fkine() {
    using namespace my_math;
    using namespace roboarm_dep;
    float q1 = real_relative_pos.joint1 * d2r;
    float q2 = real_relative_pos.joint2 * d2r;
    float q3 = real_relative_pos.joint3 * d2r;
    pos[0] = arm_cos_f32(q1) * (A * arm_sin_f32(q2 + q3) + B * arm_sin_f32(q2));
    pos[1] = arm_sin_f32(q1) * (A * arm_sin_f32(q2 + q3) + B * arm_sin_f32(q2));
    pos[2] = A * arm_cos_f32(q2 + q3) + B * arm_cos_f32(q2);
}

bool RoboArm::ikine(const float *pos) {
    using namespace my_math;
    using namespace roboarm_dep;
    uint8_t select = 0b00001111;

    float q1_slove[2];
    float q3_slove[2];

    float q2_slove[4];

    float destion[4];
    /*
     * 1. 计算q1的解
     */
    if (is_zero(pos[0]) && is_zero(pos[1])) {
        if (abs((pos[2])) <= A + B) {
            q1_slove[0] = real_relative_pos.joint1;
            q1_slove[1] = real_relative_pos.joint1;
            //            return false;
        } else {
            select = 0;
            return false;
        }
    } else {
        if (is_zero(pos[0])) {
            q1_slove[0] = pi / 2;
            q1_slove[1] = -pi / 2;
        } else if (is_zero(pos[1])) {
            q1_slove[1] = 0;
            q1_slove[0] = -pi;
        } else {
            q1_slove[0] = arm_atan_f32(pos[1], pos[0]);
        }

        if (q1_slove[0] < 0) {
            q1_slove[1] = q1_slove[0] + pi;
        } else {
            q1_slove[1] = q1_slove[0];
            q1_slove[0] -= pi;
        }
    }
    /*
     * 2. 计算q3的解，判断是否有解
     */
    float delta;
    if (is_zero(pos[0])) {
        delta = (pos[2] * pos[2] + pos[1] / arm_sin_f32(q1_slove[0]) * pos[1] / arm_sin_f32(q1_slove[0]) - A * A -
                 B * B) / (2 * A * B);
    } else {
        delta = (pos[2] * pos[2] + pos[0] / arm_cos_f32(q1_slove[0]) * pos[0] / arm_cos_f32(q1_slove[0]) - A * A -
                 B * B) / (2 * A * B);
    }
    if (delta > 1) {
        select = 0;
        return false;
    }
    q3_slove[1] = arm_acos_f32(delta);
    q3_slove[0] = -q3_slove[1];

    /*
     * 3. 计算q2的解
     */
    q2_slove[0] = slove_q2(q1_slove[0], q3_slove[0], pos[0], pos[1], pos[2]);
    q2_slove[1] = slove_q2(q1_slove[0], q3_slove[1], pos[0], pos[1], pos[2]);
    q2_slove[2] = slove_q2(q1_slove[1], q3_slove[0], pos[0], pos[1], pos[2]);
    q2_slove[3] = slove_q2(q1_slove[1], q3_slove[1], pos[0], pos[1], pos[2]);


    /*
     * 4. 组合解,并判断解是否在限制范围内
     */
    if (isInRange(q1_slove[0], limitation.joint1.min * b22r, limitation.joint1.max * b22r, err) &&
        isInRange(q2_slove[0], limitation.joint2.min * b22r, limitation.joint2.max * b22r, err) &&
        isInRange(q3_slove[0], limitation.joint3.min * b22r, limitation.joint3.max * b22r, err)) {
        //        q_sols[0][0] = q1_slove[0];
        //        q_sols[0][2] = q3_slove[0];
        //        q_sols[0][1] = q2_slove[0];
    } else {
        select &= 0b1110;
    }

    if (isInRange(q1_slove[0], limitation.joint1.min * b22r, limitation.joint1.max * b22r, err) &&
        isInRange(q2_slove[1], limitation.joint2.min * b22r, limitation.joint2.max * b22r, err) &&
        isInRange(q3_slove[1], limitation.joint3.min * b22r, limitation.joint3.max * b22r, err)) {
        //        q_sols[1][0] = q1_slove[0];
        //        q_sols[1][2] = q3_slove[1];
        //        q_sols[1][1] = q2_slove[1];
    } else {
        select &= 0b1101;
    }

    if (isInRange(q1_slove[1], limitation.joint1.min * b22r, limitation.joint1.max * b22r, err) &&
        isInRange(q2_slove[2], limitation.joint2.min * b22r, limitation.joint2.max * b22r, err) &&
        isInRange(q3_slove[0], limitation.joint3.min * b22r, limitation.joint3.max * b22r, err)) {
        //        q_sols[2][0] = q1_slove[1];
        //        q_sols[2][2] = q3_slove[0];
        //        q_sols[2][1] = q2_slove[2];
    } else {
        select &= 0b1011;
    }

    if (isInRange(q1_slove[1], limitation.joint1.min * b22r, limitation.joint1.max * b22r, err) &&
        isInRange(q2_slove[3], limitation.joint2.min * b22r, limitation.joint2.max * b22r, err) &&
        isInRange(q3_slove[1], limitation.joint3.min * b22r, limitation.joint3.max * b22r, err)) {
        //        q_sols[3][0] = q1_slove[1];
        //        q_sols[3][2] = q3_slove[1];
        //        q_sols[3][1] = q2_slove[3];
    } else {
        select &= 0b0111;
    }
    destion[0] = (q1_slove[0] - real_relative_pos.joint1 * d2r) * (q1_slove[0] - real_relative_pos.joint1 * d2r) +
                 (q2_slove[0] - real_relative_pos.joint2 * d2r) * (q2_slove[0] - real_relative_pos.joint2 * d2r) +
                 (q3_slove[0] - real_relative_pos.joint3 * d2r) * (q3_slove[0] - real_relative_pos.joint3 * d2r);
    destion[1] = (q1_slove[0] - real_relative_pos.joint1 * d2r) * (q1_slove[0] - real_relative_pos.joint1 * d2r) +
                 (q2_slove[1] - real_relative_pos.joint2 * d2r) * (q2_slove[1] - real_relative_pos.joint2 * d2r) +
                 (q3_slove[1] - real_relative_pos.joint3 * d2r) * (q3_slove[1] - real_relative_pos.joint3 * d2r);
    destion[2] = (q1_slove[1] - real_relative_pos.joint1 * d2r) * (q1_slove[1] - real_relative_pos.joint1 * d2r) +
                 (q2_slove[2] - real_relative_pos.joint2 * d2r) * (q2_slove[2] - real_relative_pos.joint2 * d2r) +
                 (q3_slove[0] - real_relative_pos.joint3 * d2r) * (q3_slove[0] - real_relative_pos.joint3 * d2r);
    destion[3] = (q1_slove[1] - real_relative_pos.joint1 * d2r) * (q1_slove[1] - real_relative_pos.joint1 * d2r) +
                 (q2_slove[3] - real_relative_pos.joint2 * d2r) * (q2_slove[3] - real_relative_pos.joint2 * d2r) +
                 (q3_slove[1] - real_relative_pos.joint3 * d2r) * (q3_slove[1] - real_relative_pos.joint3 * d2r);

    /*
     * 5. 寻找最近的解
     */
    float min_destion = 1000000000.f;
    uint8_t num = 5;  //这样如果没有解则就会保留之前的
    for (int i = 0; i < 4; ++i) {
        if (select & (1 << i)) {
            if (destion[i] < min_destion) {
                min_destion = destion[i];
                num = i;
            }
        }
    }
    select &= (1 << num);
    //    if (arm_abs_f32(max_destion) > 300 * 300) {
    //        select = 0;
    //        return false;
    //    }
    switch (select) {
        case 0b1:
            q[0] = q1_slove[0] * r2d;
            q[2] = q3_slove[0] * r2d;
            q[1] = q2_slove[0] * r2d;
            break;
        case 0b10:
            q[0] = q1_slove[0] * r2d;
            q[2] = q3_slove[1] * r2d;
            q[1] = q2_slove[1] * r2d;
            break;
        case 0b100:
            q[0] = q1_slove[1] * r2d;
            q[2] = q3_slove[0] * r2d;
            q[1] = q2_slove[2] * r2d;
            break;
        case 0b1000:
            q[0] = q1_slove[1] * r2d;
            q[2] = q3_slove[1] * r2d;
            q[1] = q2_slove[3] * r2d;
            break;
        default:
            break;
    }

    return true;
}

void RoboArm::load_target(Interact &inter) {
    using namespace roboarm_dep;
    using namespace my_math;
    switch (inter.robo_arm.mode) {
        case interact_dep::robo_mode::NORMAL:
        case interact_dep::robo_mode::VISION:
        case interact_dep::robo_mode::CUSTOM:
        case interact_dep::robo_mode::RESET:
        case interact_dep::robo_mode::ACTIONS:
            target.joint1.angle = (inter.receive_data.joint1.angle * b22d + offset.joint1) * scale(360, 36000);
//            target.joint2.angle = (inter.receive_data.joint2.angle * b22d + offset.joint2) * scale(360, 36000);
            target.joint2.internal.angle = (inter.receive_data.joint2.angle * b22d + offset.joint2.internal) * scale(360, 36000);
            target.joint2.external.angle = (inter.receive_data.joint2.angle * b22d + offset.joint2.external) * scale(360, 36000);
            //joint3是左手系 所以将右手系的数据取反
            target.joint3.angle = (-inter.receive_data.joint3.angle * b22d + offset.joint3) * scale(360, 36000);
            target.joint4.angle = (inter.receive_data.joint4.angle * b22d + offset.joint4) * scale(360, 36000);
            break;
        case interact_dep::robo_mode::XYZ:
            target.joint3.angle = (-q[2] + offset.joint3) * scale(360, 36000); //joint3是左手系 所以将右手系的数据取反
//            target.joint2.angle = (q[1] + offset.joint2) * scale(360, 36000);
            target.joint2.internal.angle = (q[1] + offset.joint2.internal) * scale(360, 36000);
            target.joint2.external.angle = (q[1] + offset.joint2.external) * scale(360, 36000);
            target.joint1.angle = (q[0] + offset.joint1) * scale(360, 36000);
            break;
    }
}

void RoboArm::load_diff_target(Interact &inter) {
    target.joint5.angle = (scale_transmit<int64_t, float>(inter.totalRoll + inter.receive_data.joint5.angle, 4096, 180));
    target.joint6.angle = scale_transmit<int64_t, float>(inter.totalRoll - inter.receive_data.joint5.angle, 4096, 180);
}
