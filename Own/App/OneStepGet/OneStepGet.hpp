//
// Created by Administrator on 25-5-1.
//

#pragma once

enum class OneStepGetYStatus {
    NONE,
    UP,
    DOWN,
};

enum class OneStepGetXStatus {
    NONE,
    FRONT,
    BACK,
};

#include "Motor/M2006Diff.hpp"
#include "Motor/M3508.hpp"
#include "Motor/Motor.hpp"
class OneStepGet {
public:
    friend void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
    friend void OneStepGetTask();

    OneStepGet(const Pid& Xcfg , const uint16_t Xid,
       const Pid& YcfgSpeed, const Pid& YcfgPos , const uint16_t Yid) : XMotor(Xcfg, Xid), YMotor(YcfgSpeed, YcfgPos, Yid), x(OneStepGetXStatus::NONE), y(OneStepGetYStatus::NONE), x_is_block(false), y_is_block(false) {};

    float move_front(float target_speed, bool is_get);
    float move_back(float target_speed);

    float move_upward(float& target_pos);
    float move_down(float& target_pos, bool is_get);

    OneStepGetXStatus x;
    bool x_is_block;
    OneStepGetYStatus y;
    bool y_is_block;
private:
    Motor<M2006Speed> XMotor;
    Motor<M3508Pos> YMotor;

};

extern OneStepGet one_step_get_left;
extern OneStepGet one_step_get_right;


