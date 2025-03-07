//
// Created by lhy on 2024/9/15.
//
#pragma once

#include "Detect/Detect.hpp"
#include "Motor/Motor.hpp"
#include "MyMath/MyMath.hpp"
#include "RemoteControl/RemoteControl.hpp"
#include "Slope/Slope.hpp"
#include "array"

namespace chassis_dep {
    struct ChassisMotorCfg {
        uint16_t id;
        Pid speed;
    };

    struct base_motor {
        Motor<SpeedPidControl<M3508>> left_front;
        Motor<SpeedPidControl<M3508>> right_front;
        Motor<SpeedPidControl<M3508>> left_rear;
        Motor<SpeedPidControl<M3508>> right_rear;

        explicit base_motor(const std::array<ChassisMotorCfg, 4>& cfg)
            : left_front(cfg[0].id, cfg[0].speed)
            , right_front(cfg[1].id, cfg[1].speed)
            , left_rear(cfg[2].id, cfg[1].speed)
            , right_rear(cfg[3].id, cfg[1].speed) {}
    };

    struct extend_motor {
        Motor<SpeedPidControl<M3508>> left;
        Motor<SpeedPidControl<M3508>> right;

        explicit extend_motor(const std::array<ChassisMotorCfg, 2>& cfg)
            : left(cfg[0].id, cfg[0].speed)
            , right(cfg[1].id, cfg[1].speed) {}
    };

    enum mode {
        ALL,
        Follow,
        Work,
        NONE,
    };

    struct move {
        float vx;
        float vy;
        float w;
        float extend;
        Slope xSlope;
        Slope ySlope;
        Slope wSlope;

        Slope extendSlope;

        explicit move(const std::array<slope_cfg, 4>& cfg)
            : vx(0)
            , vy(0)
            , w(0)
            , extend(0)
            , xSlope(cfg[0])
            , ySlope(cfg[1])
            , wSlope(cfg[2])
            , extendSlope(cfg[3]) {}
    };

    struct key {
        int8_t w;
        int8_t a;
        int8_t s;
        int8_t d;

        key()
            : w(0)
            , a(0)
            , s(0)
            , d(0) {}
    };

    enum motor_id {
        LeftFront,
        RightFront,
        LeftRear,
        RightRear,
        ExtendLeft,
        ExtendRight,
    };

    constexpr struct {
        float wheel_radius;
        float offset_x;
        float offset_y;
    } info = {76, 0, 0};

    constexpr struct {
        float wheel_base;
        float wheel_track;
    } front_info = {451, 243.74 * 2};

    constexpr struct {
        float wheel_base;
        float wheel_track;
    } rear_info = {363, 197.5 * 2};

    constexpr struct {
        float vx;
        float vy;
        float w;
    } max                 = {3732, 3732, 5};
    constexpr float v2rpm = 60 / (2 * my_math::pi * info.wheel_radius) * 19.2f;

    constexpr std::array<slope_cfg, 4> move_default = {
        slope_cfg(2, 1),
        slope_cfg(2, 1),
        slope_cfg(0.04, 0),
        slope_cfg(5.00, 1)};
    constexpr std::array<ChassisMotorCfg, 4> base_motor_default = {
        ChassisMotorCfg {1, Pid(15, 0, 4, 8000, 16000, 19.2)},
        ChassisMotorCfg {2, Pid(15, 0, 4, 8000, 16000, 19.2)},
        ChassisMotorCfg {3, Pid(15, 0, 4, 8000, 16000, 19.2)},
        ChassisMotorCfg {4, Pid(15, 0, 4, 8000, 16000, 19.2)},
    };
    constexpr std::array<ChassisMotorCfg, 2> extend_motor_default = {
        ChassisMotorCfg {5, Pid(15, 0, 4, 8000, 16000, 19.2)},
        ChassisMotorCfg {6, Pid(15, 0, 4, 8000, 16000, 19.2)},
    };

} // namespace chassis_dep
