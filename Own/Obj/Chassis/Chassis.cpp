//
// Created by lhy on 2024/9/9.
//

#include "Chassis.hpp"
#include "MyMath/MyMath.hpp"
#if USING_CHASSIS == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_math.h"

#ifdef __cplusplus
}
#endif

#include "MicroTime/MicroTime.hpp"

void Chassis::send_foc() {
    can->transmit(M3508::foc.TX_LOW_ID, base.left_front.output(), base.right_front.output(), base.right_rear.output(),
                  base.left_rear.output());
    can->transmit(M3508::foc.TX_HIGH_ID, (int16_t)extend.left.output(), (int16_t)extend.right.output(), 0, 0);
}

void Chassis::send_foc(int16_t left_front, int16_t right_front, int16_t left_rear, int16_t right_rear, int16_t left,
                       int16_t right) {
    can->transmit(M3508::foc.TX_LOW_ID, left_front, right_front, left_rear, right_rear);
    can->transmit(M3508::foc.TX_HIGH_ID, left, right, 0, 0);
}

void Chassis::send_base_foc(int16_t left_front, int16_t right_front, int16_t left_rear, int16_t right_rear) {
    can->transmit(M3508::foc.TX_LOW_ID, left_front, right_front, left_rear, right_rear);
}

void Chassis::send_extend_foc(int16_t left, int16_t right) { can->transmit(M3508::foc.TX_HIGH_ID, left, right, 0, 0); }

void Chassis::send_base_foc() {
    send_base_foc(base.left_front.output(), base.right_front.output(), base.left_rear.output(),
                  base.right_rear.output());
}

void Chassis::UpdatePid() {
    // base.left_front.motor.set_speed(base.left_front.feed_back.data.speed);
    base.left_front.set_speed(wheelSpeed[chassis_dep::LeftFront]);
    base.right_front.set_speed(wheelSpeed[chassis_dep::RightFront]);
    base.left_rear.set_speed(wheelSpeed[chassis_dep::LeftRear]);
    base.right_rear.set_speed(wheelSpeed[chassis_dep::RightRear]);

    extend.left.set_speed(wheelSpeed[chassis_dep::ExtendLeft]);
    extend.right.set_speed(wheelSpeed[chassis_dep::ExtendRight]);
}

void Chassis::UpdateMotor() {
    uint16_t id = can->read_header()->Identifier - M3508::foc.RX_ID;
    if (base.left_front.get_feedback(id, can->read())) { return; }
    if (base.right_front.get_feedback(id, can->read())) { return; }
    if (base.left_rear.get_feedback(id, can->read())) { return; }
    if (base.right_rear.get_feedback(id, can->read())) { return; }
    if (extend.left.get_feedback(id, can->read())) { return; }
    if (extend.right.get_feedback(id, can->read())) { return; }
}

void Chassis::update_slope(interact_dep::chassis_mode mode) {
    move.xSlope.update();
    move.ySlope.update();

    //    float gimbalAngleSin = arm_sin_f32(relative_angle * d2r);
    float gimbalAngleSin = 0;
    //    float gimbalAngleCos = arm_cos_f32(relative_angle * d2r);
    float gimbalAngleCos = 1;
    move.vx              = move.xSlope.get() * gimbalAngleCos + move.ySlope.get() * gimbalAngleSin;
    move.vy              = -move.xSlope.get() * gimbalAngleSin + move.ySlope.get() * gimbalAngleCos;
    move.w               = move.wSlope.update();
    move.extend          = move.extendSlope.update();
    load_speed(mode);
}

void Chassis::load_speed(interact_dep::chassis_mode mode) {
    using namespace chassis_dep;
    float rotateRatio[4];

    rotateRatio[LeftFront] = (front_info.wheel_base + front_info.wheel_track) / 2.f
                             - info.offset_y * my_abs(move.w) / max.w + info.offset_x * my_abs(move.w) / max.w;
    rotateRatio[RightFront] = (front_info.wheel_base + front_info.wheel_track) / 2.f
                              - info.offset_y * my_abs(move.w) / max.w - info.offset_x * my_abs(move.w) / max.w;
    rotateRatio[LeftRear] = (rear_info.wheel_base + rear_info.wheel_track) / 2.f
                            + info.offset_y * my_abs(move.w) / max.w + info.offset_x * my_abs(move.w) / max.w;
    rotateRatio[RightRear] = (rear_info.wheel_base + rear_info.wheel_track) / 2.f
                             + info.offset_y * my_abs(move.w) / max.w - info.offset_x * my_abs(move.w) / max.w;

    wheelSpeed[LeftFront]  = (move.vx + move.vy - rotateRatio[LeftFront] * move.w) * v2rpm;
    wheelSpeed[RightFront] = (move.vx - move.vy - rotateRatio[RightFront] * move.w) * v2rpm;
    wheelSpeed[LeftRear]   = (-move.vx + (move.vy) - rotateRatio[LeftRear] * move.w) * v2rpm;
    wheelSpeed[RightRear]  = (-move.vx - (move.vy) - rotateRatio[RightRear] * move.w) * v2rpm;

    switch (mode) {
        case interact_dep::chassis_mode::CLIMB:
            wheelSpeed[ExtendLeft]  = (move.extend) * v2rpm;
            wheelSpeed[ExtendRight] = -(move.extend) * v2rpm;
            break;
        default:
            wheelSpeed[ExtendLeft]  = (move.vy + move.extend) * v2rpm;
            wheelSpeed[ExtendRight] = -(move.vy + move.extend) * v2rpm;
            break;
    }
}

#endif
