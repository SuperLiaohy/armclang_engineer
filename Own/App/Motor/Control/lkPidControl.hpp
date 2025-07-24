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
    lkPidControl(const Pid& pos_cfg,const Pid& speed_cfg,SuperCan* canPlus, uint16_t rx_id, Args&&... args)
        : motor(rx_id, std::forward<Args>(args)...)
        , position(pos_cfg)
        , speed(speed_cfg)
    , tx_id(rx_id)
    , canPlus(canPlus){};

    [[nodiscard]] float& output() { return this->position.output; }
    [[nodiscard]] float& speed_output() { return this->speed.output; }

    float set_position(const float target) {
        if (!this->detect.isLost) {
            return this->speed.update(this->position.update(target, this->total_position), this->feedback.data.speed);
        } else {
            speed.clear();
            position.clear();
            return 0;
        }
    }

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

