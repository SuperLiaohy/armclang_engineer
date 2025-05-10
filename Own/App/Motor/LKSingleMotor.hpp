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
#include "Param/LKSingleMotor.hpp"

#define USING_LKMOTOR 1
#if USING_LKMOTOR == 1


class LKMotorSingle : public LKControl<LKSingleMotor> {
public:
    template<typename... Args>
    explicit LKMotorSingle(Args&&... args)
        : LKControl(std::forward<Args>(args)...) {};

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == this->rx_id) {
            LKControl<LKSingleMotor>::get_feedback(data);
            return true;
        }
        return false;
    };
};
#include "Control/PosPidControl.hpp"
class LKPidMotor : public PosPidControl<LKSingleMotor> {
public:
    template <typename... Args>
    explicit LKPidMotor(SuperCan* canPlus,Args&&...args) : PosPidControl(std::forward<Args>(args)...), canPlus(canPlus){}
    void torqueControl() {
        int16_t target = this->speed_output();
        uint8_t data[8] = {0xa1, 0, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1), 0, 0};
        canPlus->transmit_pdata(rx_id + foc.TX_LOW_ID, data);
    }
    SuperCan* canPlus;

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == this->rx_id) {
            PosPidControl::get_feedback(data);
            return true;
        }
        return false;
    };

};

#endif //USING_LKMOTOR
