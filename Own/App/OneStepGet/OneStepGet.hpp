//
// Created by Administrator on 25-5-1.
//

#pragma once

enum class OneStepGetControl {
    AUTO,
    MANUAL,
};
enum class OneStepGetAUTO {
    NONE,
    GOT_Y,
    GOT_X,
};

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

    OneStepGet(const Pid& XcfgSpeed, const Pid& XcfgPos , const uint16_t Xid,
       const Pid& YcfgSpeed, const Pid& YcfgPos , const uint16_t Yid) : XMotor(XcfgPos, XcfgSpeed, Xid), YMotor(YcfgPos, YcfgSpeed, Yid), x(OneStepGetXStatus::NONE), y(OneStepGetYStatus::NONE), x_is_block(false), y_is_block(false) {};

    float move_front(float& target_pos, bool is_get, bool is_block);
    float move_back(float& target_pos, bool is_block);

    float move_upward(float& target_pos, bool is_block);
    float move_down(float& target_pos, bool is_get, bool is_block);

    float x_target;
    float y_target;

    OneStepGetXStatus x;
    bool x_is_block;
    OneStepGetYStatus y;
    bool y_is_block;
private:
    Motor<M2006Pos> XMotor;
    Motor<M3508Pos> YMotor;
};

extern OneStepGet one_step_get_left;
extern OneStepGet one_step_get_right;
extern OneStepGetControl one_step_get_control;
extern OneStepGetAUTO one_step_get_auto;


