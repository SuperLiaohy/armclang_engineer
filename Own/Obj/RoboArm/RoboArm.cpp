//
// Created by Administrator on 24-10-23.
//

#include "RoboArm.hpp"

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

void RoboArm::enable() {
    using namespace roboarm_dep;
    // 先清除错误状态再关闭再开启

    auto clear_error = [](auto& motor) {
        for (uint32_t i = 0; i < MaxTimeOut; i++) {
            if (motor.clear_flag) { break; }
            motor.clear_error();
            osDelay(1);
        }
    };
    clear_error(joint1);
    clear_error(joint2.internal);
    clear_error(joint2.external);
    clear_error(joint3);
    clear_error(joint4);

    auto close = [](auto& motor) {
        for (uint32_t i = 0; i < MaxTimeOut; i++) {
            if (motor.clear_flag) { break; }
            motor.clear_error();
            osDelay(1);
        }
    };
    close(joint1);
    close(joint2.internal);
    close(joint2.external);
    close(joint3);
    close(joint4);

    auto open = [](auto& motor) {
        for (uint32_t i = 0; i < MaxTimeOut; i++) {
            if (motor.start_flag) { break; }
            motor.enable();
            osDelay(1);
        }
    };
    open(joint1);
    open(joint2.internal);
    open(joint2.external);
    open(joint3);
    open(joint4);
}

void RoboArm::disable() {
    // 不会清除多圈状态
    joint1.disable();
    joint2.internal.disable();
    joint2.external.disable();
    joint3.disable();
    joint4.disable();
}

void RoboArm::close() {
    // 会清除多圈状态
    joint1.close();
    joint2.internal.close();
    joint2.external.close();
    joint3.close();
    joint4.close();
}

void RoboArm::init_offset(std::array<float, 6>& joint) {
    using namespace roboarm_dep;

    joint4.read_totalposition();
    joint3.read_totalposition();
    joint2.internal.read_totalposition();
    joint2.external.read_totalposition();
    joint1.read_totalposition();

    target.joint6.angle = 0;
    target.joint5.angle = 0;
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint4.offset_flag) {
            joint[3] = 0;
            if (joint4.feedback.total_position < 0) { offset.joint4 -= 360; }
            break;
        }
        joint4.read_totalposition();
        osDelay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint3.offset_flag) {
            joint[2] = 134;
            if (joint3.feedback.total_position < 0) { offset.joint3 -= 360; }
            break;
        }
        joint3.read_totalposition();
        osDelay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint2.internal.offset_flag) {
            joint[1] = -45;
            if (joint2.internal.feedback.total_position < 0) { offset.joint2.internal -= 360; }
            break;
        }
        joint2.internal.read_totalposition();
        osDelay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint2.external.offset_flag) {
            joint[1] = -45;
//             if (joint2.external.motor.m.feedback.total_position < 0) { offset.joint2.external -= 360; }
            break;
        }
        joint2.external.read_totalposition();
        osDelay(1);
    }
    for (uint32_t i = 0; i < MaxTimeOut; i++) {
        if (joint1.offset_flag) {
            joint[0] = 0;
            if (joint1.feedback.total_position < 0) { offset.joint1 -= 360; }
            target.joint1.angle = offset.joint1 * 100;
            break;
        }
        joint1.read_totalposition();
        osDelay(1);
    }
}

void RoboArm::update_relative_pos() {
    // relative_pos[0] = joint1.motor.m.feedback.total_position - offset.joint1;
    //
    // relative_pos[1] = -(joint2.external.motor.m.feedback.total_position - offset.joint2.external);
    //
    // relative_pos[2] = -(joint3.motor.m.feedback.total_position - offset.joint3);
    //
    // relative_pos[3] = joint4.motor.m.feedback.total_position - offset.joint4;

     relative_pos[0] = -(joint1.total_position - offset.joint1);

     relative_pos[1] = -(joint2.external.total_position - offset.joint2.external);

     relative_pos[2] = -(joint3.total_position - offset.joint3);

     relative_pos[3] = -(joint4.total_position - offset.joint4);

    diff.update_relative_pos(relative_pos[4], relative_pos[5]);

}

void RoboArm::fkine(std::array<float, 3>& xyz) {
    using namespace my_math;
    using namespace roboarm_dep;
    float q1 = relative_pos[0] * d2r;
    float q2 = relative_pos[1] * d2r;
    float q3 = relative_pos[2] * d2r;
    pos[0]   = arm_cos_f32(q1) * (A * arm_sin_f32(q2 + q3) + B * arm_sin_f32(q2));
    pos[1]   = arm_sin_f32(q1) * (A * arm_sin_f32(q2 + q3) + B * arm_sin_f32(q2));
    pos[2]   = A * arm_cos_f32(q2 + q3) + B * arm_cos_f32(q2);
    xyz[0] = pos[0];
    xyz[1] = pos[1];
    xyz[2] = pos[2];
}

bool RoboArm::ikine(const std::array<float, 3>& pos) {
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
        if (my_abs((pos[2])) <= A + B) {
            q1_slove[0] = relative_pos[0];
            q1_slove[1] = relative_pos[0];
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
            q1_slove[1]  = q1_slove[0];
            q1_slove[0] -= pi;
        }
    }
    /*
     * 2. 计算q3的解，判断是否有解
     */
    float delta;
    if (is_zero(pos[0])) {
        delta =
            (pos[2] * pos[2] + pos[1] / arm_sin_f32(q1_slove[0]) * pos[1] / arm_sin_f32(q1_slove[0]) - A * A - B * B)
            / (2 * A * B);
    } else {
        delta =
            (pos[2] * pos[2] + pos[0] / arm_cos_f32(q1_slove[0]) * pos[0] / arm_cos_f32(q1_slove[0]) - A * A - B * B)
            / (2 * A * B);
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
    if (isInRange(q1_slove[0], joint_scale<float>(limitation.joint1.min, 180, pi),
                  joint_scale<float>(limitation.joint1.max, 180, pi), err)
        && isInRange(q2_slove[0], joint_scale<float>(limitation.joint2.min, 180, pi),
                     joint_scale<float>(limitation.joint2.max, 180, pi), err)
        && isInRange(q3_slove[0], joint_scale<float>(limitation.joint3.min, 180, pi),
                     joint_scale<float>(limitation.joint3.max, 180, pi), err)) {
        destion[0] = (q1_slove[0] - relative_pos[0] * d2r) * (q1_slove[0] - relative_pos[0] * d2r)
                     + (q2_slove[0] - relative_pos[1] * d2r) * (q2_slove[0] - relative_pos[1] * d2r)
                     + (q3_slove[0] - relative_pos[2] * d2r) * (q3_slove[0] - relative_pos[2] * d2r);
    } else {
        select &= 0b1110;
    }

    if (isInRange(q1_slove[0], joint_scale<float>(limitation.joint1.min, 180, pi),
                  joint_scale<float>(limitation.joint1.max, 180, pi), err)
        && isInRange(q2_slove[1], joint_scale<float>(limitation.joint2.min, 180, pi),
                     joint_scale<float>(limitation.joint2.max, 180, pi), err)
        && isInRange(q3_slove[1], joint_scale<float>(limitation.joint3.min, 180, pi),
                     joint_scale<float>(limitation.joint3.max, 180, pi), err)) {
        destion[1] = (q1_slove[0] - relative_pos[0] * d2r) * (q1_slove[0] - relative_pos[0] * d2r)
                     + (q2_slove[1] - relative_pos[1] * d2r) * (q2_slove[1] - relative_pos[1] * d2r)
                     + (q3_slove[1] - relative_pos[2] * d2r) * (q3_slove[1] - relative_pos[2] * d2r);

    } else {
        select &= 0b1101;
    }

    if (isInRange(q1_slove[1], joint_scale<float>(limitation.joint1.min, 180, pi),
                  joint_scale<float>(limitation.joint1.max, 180, pi), err)
        && isInRange(q2_slove[2], joint_scale<float>(limitation.joint2.min, 180, pi),
                     joint_scale<float>(limitation.joint2.max, 180, pi), err)
        && isInRange(q3_slove[0], joint_scale<float>(limitation.joint3.min, 180, pi),
                     joint_scale<float>(limitation.joint3.max, 180, pi), err)) {
        destion[2] = (q1_slove[1] - relative_pos[0] * d2r) * (q1_slove[1] - relative_pos[0] * d2r)
                     + (q2_slove[2] - relative_pos[1] * d2r) * (q2_slove[2] - relative_pos[1] * d2r)
                     + (q3_slove[0] - relative_pos[2] * d2r) * (q3_slove[0] - relative_pos[2] * d2r);

    } else {
        select &= 0b1011;
    }

    if (isInRange(q1_slove[1], joint_scale<float>(limitation.joint1.min, 180, pi),
                  joint_scale<float>(limitation.joint1.max, 180, pi), err)
        && isInRange(q2_slove[3], joint_scale<float>(limitation.joint2.min, 180, pi),
                     joint_scale<float>(limitation.joint2.max, 180, pi), err)
        && isInRange(q3_slove[1], joint_scale<float>(limitation.joint3.min, 180, pi),
                     joint_scale<float>(limitation.joint3.max, 180, pi), err)) {
        destion[3] = (q1_slove[1] - relative_pos[0] * d2r) * (q1_slove[1] - relative_pos[0] * d2r)
                     + (q2_slove[3] - relative_pos[1] * d2r) * (q2_slove[3] - relative_pos[1] * d2r)
                     + (q3_slove[1] - relative_pos[2] * d2r) * (q3_slove[1] - relative_pos[2] * d2r);
    } else {
        select &= 0b0111;
    }

    /*
     * 5. 寻找最近的解
     */
    float min_destion = 1000000000.f;
    uint8_t num       = 5; //这样如果没有解则就会保留之前的
    for (int i = 0; i < 4; ++i) {
        if (select & (1 << i)) {
            if (destion[i] < min_destion) {
                min_destion = destion[i];
                num         = i;
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
        default: break;
    }

    return true;
}

void RoboArm::load_target(const std::array<float, 6>& joint) {
    using namespace roboarm_dep;
    using namespace my_math;

    //电机的转向和人为规定的全部反了,故加上了负号.
    target.joint1.angle          = (-joint[0] + offset.joint1) * scale(360, 36000);
    target.joint2.internal.angle = (-joint[1] + offset.joint2.internal) * scale(360, 36000);
    target.joint2.external.angle = (-joint[1] + offset.joint2.external) * scale(360, 36000);
    target.joint3.angle          = (-joint[2] + offset.joint3) * scale(360, 36000);
    target.joint4.angle          = (-joint[3] + offset.joint4) * scale(360, 36000);
    auto data = joint[5];
    float err = joint[5] - relative_pos[5]; // 10 <- 350 + 360 = -340 - 360 // 350 <- 10 = 340
    while (err > 180) { err -= 360; }
    while (err < -180) { err += 360; }
    data = relative_pos[5] + err;
    target.joint5.angle = data + joint[4];
    target.joint6.angle = -data + joint[4];
}

