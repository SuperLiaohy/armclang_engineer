//
// Created by lhy on 2024/9/9.
//
#pragma once

#include "Motor.hpp"
#define USING_M3508 1
#if USING_M3508 == 1


class M3508 : public default_motor<8192, 19> {
public:
    M3508(const uint16_t rx_id)
        : default_motor(rx_id) {};

    static constexpr FOC foc = {0x200, 0x200, 0x1FF};;
};


template<motor_param motor>
class SpeedPidControl {
public:
    SpeedPidControl(uint16_t rx_id)
        : m(rx_id)
        , speed(Pid()) {};

    void init(float p, float i, float d, float maxI, float maxOut, float gain);

    void set_speed(float target) {
        speed.update(target, m.feedback.data.speed);
    }

    [[nodiscard]] float output() const { return speed.output; }

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == m.rx_id) {
            m.get_feedback(data);
            return true;
        }
        return false;
    };

private:
    motor m;
    Pid speed;
};

#endif //USING_M3508
