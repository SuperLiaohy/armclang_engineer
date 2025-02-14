//
// Created by Administrator on 25-1-20.
//

#ifndef ARMCLANG_ENGINEER_IMAGETRANS_H
#define ARMCLANG_ENGINEER_IMAGETRANS_H

#include "Uart/SuperUart.h"
#include "Key/Key.h"
#include "Crc/Crc.h"
#include "array"
#include "RoboArm/roboarm_dep.h"

namespace image_trans_dep {
    constexpr uint8_t SOF = 0xA5; // 帧头

    struct rx_status {
        uint8_t map_back_over: 1;
        uint8_t lock: 1;
        uint8_t none: 6;
    } __attribute__((packed));

    struct custom_rx_frame {
        rx_status s;
        std::array<int16_t, 6> joint;
    } __attribute__((packed));

    struct tx_status {
        uint8_t enable_map_back: 1;
        uint8_t lock: 1;
        uint8_t none: 6;
    } __attribute__((packed));

    struct custom_tx_frame {
        tx_status s;
        std::array<int16_t, 6> joint;
    } __attribute__((packed));


    struct frame_header {
        uint8_t sof;
        uint16_t data_length;
        uint8_t seq;
        uint8_t crc8;
    } __attribute__((packed));


    struct trans_frame {
        frame_header frame_head{};
        uint16_t cmd_id{};
        uint8_t data[30]{};
        uint16_t crc16{};
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
    ImageTrans(UART_HandleTypeDef *huart) : uartPlus(huart, 100, 100) {
        tx_frame.frame_head = {0xA5, 30, 0, 0};
        crc::append_crc8_check_sum(reinterpret_cast<uint8_t*>(&tx_frame.frame_head), sizeof(image_trans_dep::frame_header));
        tx_frame.cmd_id = 0x302;
    }

    void set_map_back(uint8_t is_able) { custom_tx_frame.s.enable_map_back = is_able; };

    uint8_t read_map_back() { return custom_tx_frame.s.enable_map_back; };

    void transmit();

    void get_angle(roboarm_dep::real_relative_pos &relativePos);

    void update();

    image_trans_dep::custom_rx_frame custom_rx_frame;
    image_trans_dep::trans_frame tx_frame;

    image_trans_dep::frame_header rx_header;//帧头
    uint16_t rx_cmd_id;//命令ID

    SuperUart uartPlus;

private:
    image_trans_dep::custom_tx_frame custom_tx_frame;

//    void update_custom();
//
//    void update_keyboard();
//
//    void transmit_custom(uint32_t cmd_id, uint8_t *data, uint16_t len);

};


#endif //ARMCLANG_ENGINEER_IMAGETRANS_H
