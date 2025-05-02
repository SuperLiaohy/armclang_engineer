/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 17:30:31
 * @FilePath: /DM/Own/App/Motor/M2006.hpp
 * @Description: DJI 2006电机 使用的C610的FOC
 */
#pragma once

#include "../Motor.tpp"
#define USING_M2006 1
#if USING_M2006 == 1

class M2006Diff : public default_motor<8192, 36 * 20, 1> {
public:
    explicit M2006Diff(const uint16_t rx_id)
        : default_motor(rx_id)
        , cnt(0) {};

    int32_t cnt;

    static constexpr FOC foc = {0x200, 0x200, 0x1FF};
    float total_cnt() {
        float dPos = feedback.data.position - feedback.data.last_position;
        if (dPos > 180) {
            ++cnt;
        } else if (dPos < -180) {
            --cnt;
        }
        return dPos;
    }
};

class M2006 : public default_motor<8192, 36, 1> {
public:
    explicit M2006(const uint16_t rx_id)
        : default_motor(rx_id)
        , cnt(0) {};

    int32_t cnt;

    static constexpr FOC foc = {0x200, 0x200, 0x1FF};
    float total_cnt() {
        float dPos = feedback.data.position - feedback.data.last_position;
        if (dPos > 180) {
            ++cnt;
        } else if (dPos < -180) {
            --cnt;
        }
        return dPos;
    }
};

#endif //USING_M2006
