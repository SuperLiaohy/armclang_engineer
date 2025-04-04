//
// Created by Administrator on 25-1-6.
//
#pragma once

#include "Matrix/Matrix.hpp"
#include "Motor/Motor.hpp"
#include "MyMath/MyMath.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_math.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#ifdef __cplusplus
}
#endif

namespace roboarm_dep {
    struct range {
        float min;
        float max;
    };

    struct joint_target {
        float angle;
        float speed;
    };

    struct target {
        joint_target joint1;

        //        joint_target joint2;
        struct {
            joint_target internal;
            joint_target external;
        } joint2;

        joint_target joint3;
        joint_target joint4;
        joint_target joint5;
        joint_target joint6;
    };

    struct offset {
        float joint1;
        //        float joint2;
        struct {
            float internal;
            float external;
        } joint2;
        float joint3;
        float joint4;
        float joint5;
        float joint6;
    };

    class MDH {
    public:
        MDH() = default;

        MDH(float a, float alpha, float d, float theta = 0) {
            this->a     = a;
            this->alpha = alpha;
            this->d     = d;
            this->theta = theta;
            rota        = {
                {arm_cos_f32(theta), -arm_sin_f32(theta), 0},
                {arm_sin_f32(theta) * arm_cos_f32(alpha), arm_cos_f32(theta) * arm_cos_f32(alpha), -arm_sin_f32(alpha)},
                {arm_sin_f32(theta) * arm_sin_f32(alpha), arm_cos_f32(theta) * arm_sin_f32(alpha), arm_cos_f32(alpha)}};
            trans = {
                {a},
                {-arm_sin_f32(alpha) * d},
                {arm_cos_f32(alpha) * d}};
        }

        float a;
        float alpha;
        float d;
        float theta;

        Matrix<3, 3> rota;
        Matrix<3, 1> trans;

        MDH inv();

        inline void change_theta(float theta);

        inline void change_d(float d);
    };

    inline void MDH::change_theta(float theta) {
        rota.data[0][0] = cosf(theta);
        rota.data[0][1] = -arm_sin_f32(theta);
        rota.data[1][0] = arm_sin_f32(theta) * arm_cos_f32(alpha);
        rota.data[1][1] = arm_cos_f32(theta) * arm_cos_f32(alpha);
        rota.data[2][0] = arm_sin_f32(theta) * arm_sin_f32(alpha);
        rota.data[2][1] = -arm_cos_f32(theta) * arm_sin_f32(alpha);

        this->theta = theta;
    }

    inline void MDH::change_d(float d) {
        trans.data[1][0] = d * arm_sin_f32(alpha);
        trans.data[2][0] = d * arm_sin_f32(alpha);
        this->d          = d;
    }

    class link {
    public:
        MDH dh;
    };

    class Differentiator {
    public:
        float common     = 0;
        float difference = 0;
        float lAngle     = 0;
        float rAngle     = 0;
        float gain;
        Motor<PosPidControl<M2006>> left;
        Motor<PosPidControl<M2006>> right;

        Differentiator(float gain,
                       uint32_t left_id, const Pid& left_pos_pid, const Pid& left_speed_pid,
                       uint32_t right_id, const Pid& right_pos_pid, const Pid& right_speed_pid)
            : gain(gain)
            , left(left_id, left_pos_pid, left_speed_pid)
            , right(right_id, right_pos_pid, right_speed_pid) {};

        void init();

        void update_relative_pos(float& pitch, float& roll);
    };

    constexpr float err           = deg2rad(5);
    constexpr float A             = 340.0;
    constexpr float B             = 330.0;
    constexpr uint32_t MaxTimeOut = 3000;
    constexpr float M2006Scale    = 8192.0 * 36;

    template<typename T = int16_t>
    consteval T joint_scale(float angle, float scr, float head) {
        return static_cast<T>(angle * head / scr);
    }

    consteval float joint_scale(float angle, float scale) {
        return static_cast<float>(angle * scale);
    }

    inline bool is_zero(float x) {
        return fabsf(x) < 1e-6;
    };

    inline float arm_atan_f32(float y, float x) {
        return atanf(y / x);
    };

    inline float arm_atan2_f32(float y, float x) {
        return atan2f(y, x);
    };

    inline float arm_acos_f32(float x) {
        return acosf(x);
    };

    inline float arm_abs_f32(float x) {
        return fabsf(x);
    };

    inline float arm_asin_f32(float x) {
        return asinf(x);
    };

    inline bool is_equal(float x, float y) {
        return is_zero(x - y);
    };
    constexpr struct {
        range joint1;
        range joint2;
        range joint3;
        range joint4;
        range joint5;
        range joint6;
    } limitation = {
        {-45, 45},
        {-45 , 45},
        {-135, 135},
        {-180, 179.9},
        {-90 , 90},
        {-90 , 90}};

} // namespace roboarm_dep
