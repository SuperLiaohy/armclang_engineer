//
// Created by Administrator on 2025/3/4.
//

#pragma once

#include "M3508.hpp"

template<motor_control MOTOR>
class Motor {
public:
    Motor(const uint16_t rx_id) requires std::same_as<MOTOR, SpeedPidControl<M3508>> : motor(rx_id) {};
    MOTOR motor;
};



Motor<SpeedPidControl<M3508>> dsa(1);
