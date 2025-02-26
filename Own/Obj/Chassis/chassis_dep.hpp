//
// Created by lhy on 2024/9/15.
//
#pragma once


#include "Detect/Detect.hpp"
#include "Motor/M3508.hpp"
#include "Motor/Motor.hpp"
#include "MyMath/MyMath.hpp"
#include "RemoteControl/RemoteControl.hpp"
#include "Slope/Slope.hpp"
#include "array"

namespace chassis_dep {
    struct base_motor {
        Motor<M3508> left_front;
        Motor<M3508> right_front;
        Motor<M3508> left_rear;
        Motor<M3508> right_rear;

        explicit base_motor(const std::array<motor_cfg, 4> &cfg)
                : left_front(cfg[0]), right_front(cfg[1]), left_rear(cfg[2]), right_rear(cfg[3]) {}
    };

    struct extend_motor {
        Motor<M3508> left;
        Motor<M3508> right;

        explicit extend_motor(const std::array<motor_cfg, 2> &cfg)
                : left(cfg[0]), right(cfg[1]) {}
    };

    struct rotate_cfg {
        pid_cfg pid;
        const float angle;

        constexpr explicit rotate_cfg(const pid_cfg &cfg, float angle)
                : pid(cfg), angle(angle) {}
    };

    struct rotate {
        PID pid;
        const float init_angle;

        explicit rotate(const rotate_cfg &cfg)
                : pid(cfg.pid), init_angle(cfg.angle) {}
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

        explicit move(const std::array<slope_cfg, 4> &cfg)
                : vx(0), vy(0), w(0), extend(0), xSlope(cfg[0]), ySlope(cfg[1]), wSlope(cfg[2]), extendSlope(cfg[3]) {}
    };

    struct key {
        int8_t w;
        int8_t a;
        int8_t s;
        int8_t d;

        key()
                : w(0), a(0), s(0), d(0) {}
    };

    enum motor_id {
        LeftFront,
        RightFront,
        LeftRear,
        RightRear,
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
    } max = {3732, 3732, 5};
    constexpr float v2rpm = 60 / (2 * my_math::pi * info.wheel_radius) * 19.2f;

    constexpr std::array<slope_cfg, 4> move_default = {
            slope_cfg(2, 1),
            slope_cfg(2, 1),
            slope_cfg(0.04, 0),
            slope_cfg(5.00, 1)};
    constexpr std::array<motor_cfg, 4> base_motor_default = {
            motor_cfg(1, 8192),
            motor_cfg(2, 8192),
            motor_cfg(3, 8192),
            motor_cfg(4, 8192)
    };
    constexpr std::array<motor_cfg, 2> extend_motor_default = {
            motor_cfg(5, 8192),
            motor_cfg(6, 8192)};
    constexpr pid_cfg rotate_pid_default(0.15, 0, 0.1, 2, 4);
    constexpr rotate_cfg rotate_default(rotate_pid_default, 0);
} // namespace chassis_dep
