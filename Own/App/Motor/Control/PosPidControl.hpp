//
// Created by Administrator on 2025/4/8.
//

#pragma once
#include "Pid/Pid.hpp"
#include "../Motor.tpp"

template<motor_param motor>
class PosPidControl : protected motor {
public:
    template<typename... Args>
    PosPidControl(const Pid& pos_cfg,const Pid& speed_cfg, Args&&... args)
        : motor(std::forward<Args>(args)...)
        , position(pos_cfg)
        , speed(speed_cfg) {};

    [[nodiscard]] float& output() { return this->position.output; }

    float set_position(const float target) {
        return this->speed.update(this->position.update(target, this->feedback.total_position), this->feedback.data.speed/60.f*360);
    }

protected:
    Pid position;
    Pid speed;
};