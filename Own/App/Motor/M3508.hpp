//
// Created by lhy on 2024/9/9.
//
#pragma once

#define USING_M3508 1
#if USING_M3508 == 1

#include "Motor.hpp"

class M3508 : public default_motor {
public:
    M3508(const uint16_t rx_id, const float reduction_ratio)
        : default_motor(rx_id, reduction_ratio) {};

    static const FOC foc;
};

const FOC M3508::foc = {0x200, 0x200, 0x1FF};

template<motor_param motor>
class SpeedPidControl {
public:
    SpeedPidControl()
        : speed(Pid()) {};

    void init(float p, float i, float d, float maxI, float maxOut, float gain);

    void set_position(float target) {};

    void set_speed(float target) {
        speed.update(target, m.feedback.data.speed);
    }
    void set_current(float target) {};

    void get_feedback(uint16_t id, uint8_t* data) {
        if (id == m.rx_id) {
            m.get_feedback(data);
        }
    };

private:
    motor m;
    Pid speed;
};




#endif //USING_M3508
