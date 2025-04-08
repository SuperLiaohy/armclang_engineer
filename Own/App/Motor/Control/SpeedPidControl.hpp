//
// Created by Administrator on 2025/4/8.
//

#pragma once
#include "Pid/Pid.hpp"
#include "../Motor.tpp"

template<motor_param motor>
class SpeedPidControl : protected motor {
public:
    template<typename... Args>
    explicit SpeedPidControl(const Pid& cfg, Args&&... args)
        : motor(std::forward<Args>(args)...)
        , speed(cfg) {};

    void set_speed(float target) {
        speed.update(target, this->feedback.data.speed);
    }

    [[nodiscard]] float output() const { return speed.output; }

private:
    Pid speed;
};
