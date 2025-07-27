/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 15:21:52
 * @FilePath: /DM/Own/App/Motor/lkMotor.hpp
 * @Description: 
 * Copyright (c) 2024 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#pragma once

#include "Control/lkControl.hpp"
#include "Control/lkPidControl.hpp"
#include "Param/lkMotor.hpp"
#include <MyMath/MyMath.hpp>
#define USING_LKMOTOR 1
#if USING_LKMOTOR == 1

class LKMotorSingle : public LKControl<LKMotor> {
public:
    template<typename... Args>
    explicit LKMotorSingle(Args&&... args)
        : LKControl(std::forward<Args>(args)...) {};

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == this->rx_id) {
            LKMotor::get_feedback(data);
            return true;
        }
        return false;
    };
};
class LKMotorPid : public lkPidControl<LKMotor> {
public:
    template<typename... Args>
    explicit LKMotorPid(Args&&... args)
        : lkPidControl(std::forward<Args>(args)...) {};

    void change_speed_kp(float kp) {
        speed.parameters.p = kp;
    }
    void change_speed_ki(float ki) {
        speed.parameters.i = ki;
    }
    void change_speed_kd(float kd) {
        speed.parameters.d = kd;
    }

    void change_pos_kp(float kp) {
        position.parameters.p = kp;
    }
    void change_pos_ki(float ki) {
        position.parameters.i = ki;
    }
    void change_pos_kd(float kd) {
        position.parameters.d = kd;
    }



    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == this->rx_id) {
            LKMotor::get_feedback(data);
            return true;
        }
        return false;
    };
};


#endif //USING_LKMOTOR
