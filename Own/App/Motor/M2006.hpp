/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 17:30:31
 * @FilePath: /DM/Own/App/Motor/M2006.hpp
 * @Description: DJI 2006电机 使用的C610的FOC
 */
#pragma once

#include "Motor.tpp"
#include "Pid/Pid.hpp"
#define USING_M2006 1
#if USING_M2006 == 1

class M2006 : public default_motor<8192, 36 * 20, 1> {
public:
    M2006(const uint16_t rx_id)
        : default_motor(rx_id) {};

    int32_t cnt;

    static constexpr FOC foc = {0x200, 0x200, 0x1FF};
    [[nodiscard]] float total_cnt() {
        float dPos = feedback.data.position - feedback.data.last_position;
        if (dPos > 180) {
            ++cnt;
        } else if (dPos < -180) {
            --cnt;
        }
        return dPos;
    }
};

template<motor_param motor>
class PosPidControl {
public:
    PosPidControl(uint16_t rx_id,const Pid& pos_cfg,const Pid& speed_cfg)
        : m(rx_id)
        , position(pos_cfg)
        , speed(speed_cfg) {};

    float set_position(const float target) {
        return speed.update(position.update(target, m.feedback.total_position), m.feedback.data.speed/60.f*360);
    }

    void clear() {
        position.clear();
        speed.clear();
    }

    [[nodiscard]] float& output() { return position.output; }
    [[nodiscard]] float& total_position() { return m.feedback.total_position; }

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == m.rx_id) {
            m.get_feedback(data);
            m.total_cnt();
            return true;
        }
        return false;
    };

private:
    motor m;
    Pid position;
    Pid speed;
};

#endif //USING_M2006
