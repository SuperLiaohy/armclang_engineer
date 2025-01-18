//
// Created by liaohy on 9/5/24.
//

#ifndef NONE_PRJ_SUPERCAN_H
#define NONE_PRJ_SUPERCAN_H


#ifdef __cplusplus
extern "C" {
#endif

#include "fdcan.h"

#ifdef __cplusplus
}
#endif


enum filter_mode {
    RANGE,
    MASK,
    LIST,
};

class SuperCan {
public:
    SuperCan(FDCAN_HandleTypeDef *_hcan, uint32_t _fifo, uint32_t _fifo_start);

    ~SuperCan() = default;

    void filter_config(uint16_t filter_number, filter_mode filterMode = MASK, uint32_t reg_1 = 0, uint32_t reg_2 = 0);

    void can_start();

    void write(const uint8_t *data);

    void write(int16_t data1, int16_t data2, int16_t data3, int16_t data4);

    void send(uint32_t id, uint8_t *data);

    void send(uint32_t id);


    void receive();

    FDCAN_RxHeaderTypeDef* read_header();

    uint8_t* read();

    friend void::HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);

    friend void::HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs);

private:
    uint32_t fifo;
    uint32_t fifo_start;

    FDCAN_HandleTypeDef *hcan;
    FDCAN_TxHeaderTypeDef tx_header{};
    FDCAN_RxHeaderTypeDef rx_header{};

    uint8_t tx_data[8]{};

    uint8_t rx_data[8]{};

};

inline void SuperCan::write(const uint8_t *data) {
    for (int i = 0; i < 8; i++) {
        tx_data[i] = data[i];
    }
}

inline void SuperCan::write(int16_t data1, int16_t data2, int16_t data3, int16_t data4) {
    tx_data[0] = (data1 >> 8) & 0xff;
    tx_data[1] = (data1) & 0xff;
    tx_data[2] = (data2 >> 8) & 0xff;
    tx_data[3] = (data2) & 0xff;
    tx_data[4] = (data3 >> 8) & 0xff;
    tx_data[5] = (data3) & 0xff;
    tx_data[6] = (data4 >> 8) & 0xff;
    tx_data[7] = (data4) & 0xff;
}


inline void SuperCan::send(uint32_t id)  {
    tx_header.Identifier = id;
    tx_header.IdType = FDCAN_STANDARD_ID;
    tx_header.DataLength = 8;
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    tx_header.BitRateSwitch = FDCAN_BRS_OFF;
    tx_header.FDFormat = FDCAN_CLASSIC_CAN;
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    tx_header.MessageMarker = 0;
    if (HAL_FDCAN_AddMessageToTxFifoQ(hcan, &tx_header, tx_data) != HAL_OK) {
        return;
    }
}

inline void SuperCan::receive() {
    HAL_FDCAN_GetRxMessage(hcan, fifo, &rx_header, rx_data);
}

inline FDCAN_RxHeaderTypeDef* SuperCan::read_header() {
    return &rx_header;
}

inline uint8_t* SuperCan::read()  {
    return rx_data;
}


extern SuperCan canPlus1;
extern SuperCan canPlus2;
extern SuperCan canPlus3;

#endif //NONE_PRJ_SUPERCAN_H
