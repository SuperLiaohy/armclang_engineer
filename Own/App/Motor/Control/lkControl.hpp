//
// Created by Administrator on 2025/4/8.
//

#pragma once
#include "MyMath/MyMath.hpp"
#include "../Motor.tpp"
#include "CAN/SuperCan.hpp"
template<motor_param motor>
class LKControl : protected motor {
public:
    template<typename... Args>
    LKControl(SuperCan* canPlus, uint16_t rx_id, Args&&... args)
        : motor(rx_id, std::forward<Args>(args)...)
        , tx_id(rx_id)
        , canPlus(canPlus) {};

    void set_position(float position, float speed = 100) {
        ++tx_cnt;
        float err = position - this->feedback.total_position * 100; // 10 <- 350 + 360 = -340 - 360 // 350 <- 10 = 340
        while (err > 18000) { err -= 36000; }
        while (err < -18000) { err += 36000; }
        // position = m.feedback.total_position * 100 + err;
        totalposition2Control(limited<float>((speed * my_abs(err)) / 1000, 0.5 * speed, 1.5 * speed),
                              position * this->reduction_ratio);
    };

    void set_position_near(float position, float speed = 100) {
        ++tx_cnt;
        float err = position - this->feedback.total_position * 100; // 10 <- 350 + 360 = -340 - 360 // 350 <- 10 = 340
        while (err > 18000) { err -= 36000; }
        while (err < -18000) { err += 36000; }
        position = this->feedback.total_position * 100 + err;
        totalposition2Control(limited<float>((speed * my_abs(err)) / 1000, 0.5f * speed, 3 * speed),
                              position * this->reduction_ratio);
    };

    void read_totalposition();

    void enable();

    void disable();

    void close();

    friend class RoboArm;
    void clear_error();

private:
    Count tx_cnt;
    uint16_t tx_id;

    SuperCan* canPlus;

    void voltageControl(int16_t target);

    void torqueControl(int16_t target);

    void speedControl(int32_t target);

    void totalposition1Control(int32_t target);

    void totalposition2Control(uint16_t speed, int32_t totalposition);

    void position1Control(uint8_t spin, uint32_t target);

    void position2Control(uint8_t spin, uint16_t speed, uint32_t target);

    void addposition1Control(int32_t target);

    void addposition2Control(uint16_t speed, int32_t target);

    void read_feedback();

    void read_encoder();

    // void read_totalposition();

    void readPid();

    void writeRAMPid();

    void writeROMPid();

    void read_error();

};

template<motor_param motor> void LKControl<motor>::enable() {
    ++tx_cnt;
    uint8_t data[8] = {0x88, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::disable() {
    ++tx_cnt;
    uint8_t data[8] = {0x81, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::close() {
    ++tx_cnt;
    uint8_t data[8] = {0x80, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::voltageControl(int16_t target) {
    uint8_t data[8] = {0xa0, 0, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1), 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::torqueControl(int16_t target) {
    uint8_t data[8] = {0xa1, 0, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1), 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::speedControl(int32_t target) {
    uint8_t data[8] = {0xa2,
                       0,
                       0,
                       0,
                       *(uint8_t*)(&target),
                       *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2),
                       *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::totalposition1Control(int32_t target) {
    uint8_t data[8] = {0xa3,
                       0,
                       0,
                       0,
                       *(uint8_t*)(&target),
                       *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2),
                       *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::totalposition2Control(uint16_t speed, int32_t totalposition) {
    uint8_t data[8] = {0xa4,
                       0,
                       *(uint8_t*)(&speed),
                       *((uint8_t*)(&speed) + 1),
                       *(uint8_t*)(&totalposition),
                       *((uint8_t*)(&totalposition) + 1),
                       *((uint8_t*)(&totalposition) + 2),
                       *((uint8_t*)(&totalposition) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::position1Control(uint8_t spin, uint32_t target) {
    uint8_t data[8] = {0xa5,
                       spin,
                       0,
                       0,
                       *(uint8_t*)(&target),
                       *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2),
                       *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::position2Control(uint8_t spin, uint16_t speed, uint32_t target) {
    uint8_t data[8] = {0xa6,
                       spin,
                       *(uint8_t*)(&speed),
                       *((uint8_t*)(&speed) + 1),
                       *(uint8_t*)(&target),
                       *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2),
                       *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::addposition1Control(int32_t target) {
    uint8_t data[8] = {0xa7,
                       0,
                       0,
                       0,
                       *(uint8_t*)(&target),
                       *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2),
                       *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::addposition2Control(uint16_t speed, int32_t target) {
    uint8_t data[8] = {0xa8,
                       0,
                       *(uint8_t*)(&speed),
                       *((uint8_t*)(&speed) + 1),
                       *(uint8_t*)(&target),
                       *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2),
                       *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::read_feedback() {
    uint8_t data[8] = {0x9c, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::read_encoder() {
    uint8_t data[8] = {0x90, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::readPid() {
    uint8_t data[8] = {0x30, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::writeRAMPid() {
    uint8_t data[8] = {0x31,
                       0,
                       this->feedback.foc_pid.pos_p,
                       this->feedback.foc_pid.pos_i,
                       this->feedback.foc_pid.speed_p,
                       this->feedback.foc_pid.speed_i,
                       this->feedback.foc_pid.current_p,
                       this->feedback.foc_pid.current_i};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::writeROMPid() {
    uint8_t data[8] = {0x32,
                       0,
                       this->feedback.foc_pid.pos_p,
                       this->feedback.foc_pid.pos_i,
                       this->feedback.foc_pid.speed_p,
                       this->feedback.foc_pid.speed_i,
                       this->feedback.foc_pid.current_p,
                       this->feedback.foc_pid.current_i};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::read_error() {
    uint8_t data[8] = {0x9a, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::clear_error() {
    ++tx_cnt;
    uint8_t data[8] = {0x9b, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor> void LKControl<motor>::read_totalposition() {
    ++tx_cnt;
    uint8_t data[8] = {0x92, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}
