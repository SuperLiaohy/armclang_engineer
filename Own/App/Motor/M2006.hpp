/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 17:30:31
 * @FilePath: /DM/Own/App/Motor/M2006.hpp
 * @Description: DJI 2006电机 使用的C610的FOC
 */
#pragma once

#include "Motor.hpp"
#define USING_M2006 1
#if USING_M2006 == 1

class M2006 : public default_motor<8192, 19> {
public:
    M2006(const uint16_t rx_id)
        : default_motor(rx_id) {};

    static constexpr FOC foc = {0x200, 0x200, 0x1FF};
    [[nodiscard]] float dpos() const {
        float dPos = feedback.data.position - feedback.data.last_position;
        if (dPos > 180) {
            dPos = dPos - 360;
        } else if (dPos < -180) {
            dPos = dPos + 360;
        }
        return dPos / 19.2f;
    }
};

template<motor_param motor>
class PosPidControl {
public:
    PosPidControl(uint16_t rx_id)
        : m(rx_id)
        , position(Pid())
        , speed(Pid()) {};

    void init(float p, float i, float d, float maxI, float maxOut, float gain);

    float set_position(const float target) {
        return speed.update(position.update(target, m.feedback.total_position), m.feedback.data.speed);
    }

    void clear() {
        position.clear();
        speed.clear();
    }

    // void set_total_position(float input) {
    //     m.feedback.total_position = input;
    // }

    [[nodiscard]] float& output() { return position.output; }
    [[nodiscard]] float& total_position() { return m.feedback.total_position; }
    [[nodiscard]] float dpos() { return m.dpos(); }

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == m.rx_id) {
            m.get_feedback(data);
            return true;
        }
        return false;
    };

private:
    motor m;
    Pid position;
    Pid speed;
};

// class M2006 {
// public:
//
//     M2006(uint16_t id) : doublePid(Pid(), Pid()), ctrlId(id) {};
//
//     void init(float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
//               float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut, float gain);
//
//     void readData(const uint8_t *data);
//
//     class SelfDoublePid : public DoublePid {
//     public:
//         SelfDoublePid(Pid inter_pid, Pid extern_pid) : DoublePid(inter_pid, extern_pid) {}
//
// //        float update(float extern_input, float extern_target, float inter_input) override;
//     } doublePid;
//
//     Pid pid;
//
//     const uint16_t ctrlId;
//     float gain;
// private:
//     void CalcTotalPos();
// };

#endif //USING_M2006
