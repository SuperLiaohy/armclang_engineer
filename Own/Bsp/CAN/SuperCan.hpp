//
// Created by liaohy on 9/5/24.
//
#pragma once
#include "Count/Count.hpp"
#include <concepts>
template<typename T>
concept is_can = requires(T t, uint16_t filter_number, uint32_t reg1, uint32_t reg2, typename T::filter_mode mode, const uint16_t id, const uint8_t* data, int16_t data1, int16_t data2, int16_t data3, int16_t data4) {
    typename T::tx_head;
    typename T::rx_head;
    t.filter_config(filter_number, mode, reg1, reg2);
    t.start();
    t.send_pdata(id, data);
    t.send(id, data1, data2, data3, data4);
    t.receive();
    t.receive(data);
};

#ifdef __cplusplus
extern "C" {
#endif

#include "fdcan.h"

#ifdef __cplusplus
}
#endif

namespace can_dep {
    enum class filter_mode {
        RANGE,
        MASK,
        LIST,
    };
}

class SuperCan {
public:
    using filter_mode = can_dep::filter_mode;
    using rx_head     = FDCAN_RxHeaderTypeDef;
    using tx_head     = FDCAN_TxHeaderTypeDef;

public:
    SuperCan(FDCAN_HandleTypeDef* _hcan, uint32_t _fifo, uint32_t _fifo_start);

    ~SuperCan() = default;

    void filter_config(uint16_t filter_number, filter_mode filterMode = filter_mode::MASK, uint32_t reg_1 = 0, uint32_t reg_2 = 0);

    void start();

    void transmit_pdata(uint32_t id, uint8_t* data);

    void transmit(uint32_t id, int16_t data1, int16_t data2, int16_t data3, int16_t data4);

    void receive();

    inline FDCAN_RxHeaderTypeDef* read_header();

    inline uint8_t* read();

    friend void ::HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs);

    friend void ::HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo1ITs);
    friend void ::HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs);
private:
    Count err_cnt{};
    Count rx_cnt{};
    Count tx_cnt{};

    uint32_t fifo;
    uint32_t fifo_start;

    FDCAN_HandleTypeDef* hcan;
    FDCAN_TxHeaderTypeDef tx_header {};
    FDCAN_RxHeaderTypeDef rx_header {};

    uint8_t tx_data[8] {};

    uint8_t rx_data[8] {};

};



inline void SuperCan::receive() {
    ++rx_cnt;
    HAL_FDCAN_GetRxMessage(hcan, fifo, &rx_header, rx_data);
}

inline FDCAN_RxHeaderTypeDef* SuperCan::read_header() {
    return &rx_header;
}

inline uint8_t* SuperCan::read() {
    return rx_data;
}

extern SuperCan canPlus1;
extern SuperCan canPlus2;
extern SuperCan canPlus3;
