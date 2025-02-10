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

    struct custom_frame {
        uint16_t joint1 : 11;
        uint16_t joint2 : 11;
        uint16_t joint3 : 11;
        uint16_t joint4 : 11;
        uint16_t joint5 : 11;
        uint16_t joint6 : 11;
    } __attribute__((packed));

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
    ImageTrans(UART_HandleTypeDef *huart) : uartPlus(huart, 100, 100) {}

    void update();
    image_trans_dep::custom_frame custom_frame;
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
