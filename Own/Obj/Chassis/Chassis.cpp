//
// Created by lhy on 2024/9/9.
//

#include "Chassis.h"

#if USING_CHASSIS == 1

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_math.h"

#ifdef __cplusplus
}
#endif

#include "MicroTime/MicroTime.h"

void Chassis::send_foc() {
    can->write(base.left_front.motor.pid.output, base.right_front.motor.pid.output,
               base.left_rear.motor.pid.output, base.right_rear.motor.pid.output);
    can->send(Motor<M3508>::foc.TX_LOW_ID);
//    MicroTime::us_delay(100);
    can->write((int16_t) extend.left.motor.pid.output, (int16_t) extend.right.motor.pid.output, 0, 0);
    can->send(Motor<M3508>::foc.TX_HIGH_ID);
}

void Chassis::send_foc(int16_t left_front, int16_t right_front, int16_t left_rear, int16_t right_rear, int16_t left,
                       int16_t right) {
    can->write(left_front, right_front, left_rear, right_rear);
    can->send(Motor<M3508>::foc.TX_LOW_ID);
    can->write(left, right, 0, 0);
    can->send(Motor<M3508>::foc.TX_HIGH_ID);
}

void Chassis::send_base_foc(int16_t left_front, int16_t right_front, int16_t left_rear, int16_t right_rear) {
    can->write(left_front, right_front, left_rear, right_rear);
    can->send(Motor<M3508>::foc.TX_LOW_ID);
}

void Chassis::send_extend_foc(int16_t left, int16_t right) {
    can->write(left, right, 0, 0);
    can->send(Motor<M3508>::foc.TX_HIGH_ID);
}

void Chassis::send_base_foc() {
    send_base_foc(base.left_front.motor.pid.output,
                  base.right_front.motor.pid.output,
                  base.left_rear.motor.pid.output,
                  base.right_rear.motor.pid.output);
}

void Chassis::UpdatePid() {
    base.left_front.motor.pid.update(base.left_front.feed_back.Data.speed);
    base.right_front.motor.pid.update(base.right_front.feed_back.Data.speed);
    base.left_rear.motor.pid.update(base.left_rear.feed_back.Data.speed);
    base.right_rear.motor.pid.update(base.right_rear.feed_back.Data.speed);

    extend.left.motor.pid.update(extend.left.feed_back.Data.speed);
    extend.right.motor.pid.update(extend.right.feed_back.Data.speed);
}

void Chassis::UpdateMotor() {
    uint16_t id = can->read_header()->Identifier - Motor<M3508>::foc.RX_ID;
    if (id == base.left_front.feed_back.id) {
        base.left_front.readData(can->read());

    } else if (id == base.right_front.feed_back.id) {
        base.right_front.readData(can->read());

    } else if (id == base.left_rear.feed_back.id) {
        base.left_rear.readData(can->read());

    } else if (id == base.right_rear.feed_back.id) {
        base.right_rear.readData(can->read());

    } else if (id == extend.left.feed_back.id) {
        extend.left.readData(can->read());

    } else if (id == extend.right.feed_back.id) {
        extend.right.readData(can->read());
    }
}



void Chassis::update_slope() {
    move.xSlope.update();
    move.ySlope.update();

    //    float gimbalAngleSin = arm_sin_f32(relative_angle * d2r);
    float gimbalAngleSin = 0;
    //    float gimbalAngleCos = arm_cos_f32(relative_angle * d2r);
    float gimbalAngleCos = 1;
    move.vx = move.xSlope.get() * gimbalAngleCos + move.ySlope.get() * gimbalAngleSin;
    move.vy = -move.xSlope.get() * gimbalAngleSin + move.ySlope.get() * gimbalAngleCos;
    move.w = move.wSlope.update();
    move.extend = move.vy + move.extendSlope.update();
    load_speed();
}

void Chassis::load_speed() {
    using namespace chassis_dep;
    float rotateRatio[4], wheelSpeed[4];

    rotateRatio[LeftFront] =
            (front_info.wheel_base + front_info.wheel_track) / 2.f - info.offset_y * abs(move.w) / max.w +
            info.offset_x * abs(move.w) / max.w;
    rotateRatio[RightFront] =
            (front_info.wheel_base + front_info.wheel_track) / 2.f - info.offset_y * abs(move.w) / max.w -
            info.offset_x * abs(move.w) / max.w;
    rotateRatio[LeftRear] =
            (rear_info.wheel_base + rear_info.wheel_track) / 2.f + info.offset_y * abs(move.w) / max.w +
            info.offset_x * abs(move.w) / max.w;
    rotateRatio[RightRear] =
            (rear_info.wheel_base + rear_info.wheel_track) / 2.f + info.offset_y * abs(move.w) / max.w -
            info.offset_x * abs(move.w) / max.w;

    wheelSpeed[LeftFront] =
            (move.vx + move.vy - rotateRatio[LeftFront] * move.w) * v2rpm;
    wheelSpeed[RightFront] =
            (move.vx - move.vy - rotateRatio[RightFront] * move.w) * v2rpm;
    wheelSpeed[LeftRear] =
            (-move.vx + (move.vy+move.extend) - rotateRatio[LeftRear] * move.w) * v2rpm;
    wheelSpeed[RightRear] =
            (-move.vx - (move.vy+move.extend) - rotateRatio[RightRear] * move.w) * v2rpm;

    extend.left.motor.pid.target = move.extend * v2rpm;
    extend.right.motor.pid.target = -move.extend * v2rpm;

    base.left_front.motor.pid.target = wheelSpeed[LeftFront];
    base.right_front.motor.pid.target = wheelSpeed[RightFront];
    base.left_rear.motor.pid.target = wheelSpeed[LeftRear];
    base.right_rear.motor.pid.target = wheelSpeed[RightRear];
}


#endif
