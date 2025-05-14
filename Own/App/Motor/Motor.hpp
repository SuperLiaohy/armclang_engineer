//
// Created by Administrator on 2025/3/4.
//

#pragma once
#include "M2006Diff.hpp"
#include "M3508.hpp"
#include "Motor.tpp"
#include "lkMotor.hpp"
template<motor_control motor> class Motor : public motor {
public:
    template<typename... Args>
    explicit Motor(Args&&... args)
        : motor(std::forward<Args>(args)...) {};
    void detect_lost(Fun callback) { this->detect.lostFun = callback; }
    void detect_recover(Fun callback) { this->detect.recoverFun = callback; }
};
