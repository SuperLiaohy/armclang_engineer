//
// Created by Administrator on 2025/3/4.
//

#pragma once
#include "Motor.hpp"
#include "M3508.hpp"
#include "M2006.hpp"
#include "lkMotor.hpp"

template<motor_control MOTOR>
class Motor {
public:
    Motor(const uint16_t rx_id) requires std::same_as<MOTOR, SpeedPidControl<M3508>> : motor(rx_id) {};
    Motor(const uint16_t rx_id) requires std::same_as<MOTOR, PosPidControl<M2006>> : motor(rx_id) {};
    Motor(const uint16_t rx_id, uint32_t range, float ratio, SuperCan* canPlus) requires std::same_as<MOTOR, LKControl<LKMotor>> : motor(rx_id, range, ratio, canPlus) {};
    MOTOR motor;
};



// Motor<SpeedPidControl<M3508>> dsa(1);
// Motor<PosPidControl<M2006>> dsaa(1);
