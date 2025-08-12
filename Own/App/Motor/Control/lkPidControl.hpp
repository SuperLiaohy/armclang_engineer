//
// Created by Liaoh on 25-7-8.
//

#pragma once

#include "CAN/SuperCan.hpp"
#include "Pid/Pid.hpp"
#include "../Motor.tpp"

template<motor_param motor>
class lkPidControl: public motor {
public:
    template<typename... Args>
    lkPidControl(float alpha, const Pid& pos_cfg,const Pid& speed_cfg,SuperCan* canPlus, uint16_t rx_id, Args&&... args)
        : motor(rx_id, std::forward<Args>(args)...)
        , position(pos_cfg)
        , speed(speed_cfg)
        ,alpha(alpha)
    , tx_id(rx_id)
    , canPlus(canPlus){};

    [[nodiscard]] float& output() { return this->position.output; }
    [[nodiscard]] float& speed_output() { return this->speed.output; }

    float set_position(const float target) {
        float filter = (1 - alpha) * this->feedback.data.speed + alpha * this->last_speed;
        if (!this->detect.isLost) {
            return this->speed.update(this->position.update(target, this->total_position), filter);
        } else {
            speed.clear();
            position.clear();
            return 0;
        }
    }

    void set_position(const float position, const float speed) {
        if (!this->detect.isLost) {
            // position = m.feedback.total_position * 100 + err;
            // totalposition2Control(limited<float>((speed * my_abs(err)) / 1000, 0.5 * speed, 1.5 * speed),
            //                       position * this->reduction_ratio);
            totalposition2Control(speed,position * this->reduction_ratio);
        } else {
            totalposition2Control(speed,position * this->reduction_ratio);
        }
    }
    void totalposition2Control(uint16_t speed, int32_t totalposition) {
        uint8_t data[8] = {0xa4,
                           0,
                           *(uint8_t*)(&speed),
                           *((uint8_t*)(&speed) + 1),
                           *(uint8_t*)(&totalposition),
                           *((uint8_t*)(&totalposition) + 1),
                           *((uint8_t*)(&totalposition) + 2),
                           *((uint8_t*)(&totalposition) + 3)};
        canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
    };

    void SingleControl() {
        if (!this->detect.isLost) {
            int16_t out = speed.output;
            uint8_t data[8] = {0xa1, 0, 0, 0, *(uint8_t*)(&out), *((uint8_t*)(&out) + 1), 0, 0};
            canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
        } else {
            clear_error();
            read_feedback();
        }
    };

    void enable();

    void disable();

    void close();

    void clear_error();

    void read_totalposition();

    void read_feedback();

protected:
    Pid position;
    Pid speed;

    float alpha;
    uint16_t tx_id;
    SuperCan* canPlus;
};


template<motor_param motor> void lkPidControl<motor>::enable() {
    uint8_t data[8] = {0x88, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void lkPidControl<motor>::disable() {
    uint8_t data[8] = {0x81, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void lkPidControl<motor>::close() {
    uint8_t data[8] = {0x80, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void lkPidControl<motor>::clear_error() {
    uint8_t data[8] = {0x9b, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}


template<motor_param motor> void lkPidControl<motor>::read_totalposition() {
    uint8_t data[8] = {0x92, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}


template<motor_param motor> void lkPidControl<motor>::read_feedback() {
    uint8_t data[8] = {0x9c, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

