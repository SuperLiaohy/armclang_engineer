//
// Created by Administrator on 25-1-20.
//

#ifndef ARMCLANG_ENGINEER_IMAGETRANS_H
#define ARMCLANG_ENGINEER_IMAGETRANS_H

#include "Uart/SuperUart.h"
#include "Key/Key.h"
#include "Crc/Crc.h"
namespace image_trans_dep {
    constexpr uint8_t SOF = 0xA5; // 帧头

    struct frame_header
    {
        uint8_t sof;
        uint16_t data_length;
        uint8_t seq;
        uint8_t crc8;
    } __attribute__((packed));

//    template<uint32_t len>
//    struct tx_frame {
//        frame_header header;
//        uint32_t cmd_id;
//        uint8_t data[len];
//        uint16_t tail;
//    } __attribute__((packed));
}

class ImageTrans {
public:
    ImageTrans(UART_HandleTypeDef *huart) : uartPlus(huart, 64, 0), custom_rx(), custom_tx() {}

    uint8_t custom_rx[30];
    uint8_t custom_tx[30];

    void update();

    image_trans_dep::frame_header rx_header;//帧头
    uint16_t rx_cmd_id;//命令ID

    SuperUart uartPlus;

//    void update_custom();
//
//    void update_keyboard();
//
//    void transmit_custom(uint32_t cmd_id, uint8_t *data, uint16_t len);

};


#endif //ARMCLANG_ENGINEER_IMAGETRANS_H
