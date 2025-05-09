//
// Created by Administrator on 2025/2/28.
//
#pragma once


#include "Uart/SuperUart.hpp"

namespace sub_board_dep {
#pragma pack(push, 1)
    struct frame_header {
        uint8_t sof;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct tx_status{
        uint8_t valve1: 1;
        uint8_t valve2: 1;
        uint8_t valve3: 1;
        uint8_t valve4: 1;
        uint8_t valve5: 1;
        uint8_t valve6: 1;
        uint8_t pump: 1;
        uint8_t none: 1;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct user_tx_data{
        tx_status s;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct tx_frame {
        frame_header frame_head;
        uint8_t cmd_id;
        uint8_t data[8 - sizeof(frame_header) - sizeof(uint8_t) - sizeof(uint8_t)];
        uint8_t crc8;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct rx_status {
        float valve1;
        float valve2;
        float valve3;
        float valve4;
        float valve5;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct user_rx_data{
        rx_status s;
//    float data[8];
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct rx_frame {
        frame_header frame_head;
        uint8_t cmd_id;
        uint8_t data[sizeof(user_rx_data)];
        uint8_t crc8;
    };
#pragma pack(pop)

} // sub_board_dep


class SubBoard {
public:
    using custom_rx_frame = sub_board_dep::user_rx_data;
    using receive_frame = sub_board_dep::rx_frame;
    using custom_tx_frame = sub_board_dep::user_tx_data;
    using trans_frame = sub_board_dep::tx_frame;

    SubBoard(UART_HandleTypeDef *uart) : uartPlus(uart, sizeof(receive_frame)*2, sizeof(trans_frame)) {
        rx_frame = reinterpret_cast<receive_frame*>(uartPlus.rx_buffer);
        tx_frame = reinterpret_cast<trans_frame*>(uartPlus.tx_buffer);
        tx_frame->frame_head.sof = 0xA5;
        tx_frame->cmd_id = 0x66;
        tx_frame->crc8 = 0;
    }

    SuperUart uartPlus;
    custom_rx_frame custom_frame_rx;
    receive_frame *rx_frame;

    custom_tx_frame custom_frame_tx;
    trans_frame *tx_frame;

    void set_pump(uint8_t x) { custom_frame_tx.s.pump = x; };

    void set_valve1(uint8_t x) { custom_frame_tx.s.valve1 = x; };

    void set_valve2(uint8_t x) { custom_frame_tx.s.valve2 = x; };

    void set_valve3(uint8_t x) { custom_frame_tx.s.valve3 = x; };

    void set_valve4(uint8_t x) { custom_frame_tx.s.valve4 = x; };

    void set_valve5(uint8_t x) { custom_frame_tx.s.valve5 = x; };

    void set_valve6(uint8_t x) { custom_frame_tx.s.valve6 = x; };

    uint8_t read_tx_status(){ return *reinterpret_cast<uint8_t*>(&custom_frame_tx.s);}

    void transmit();

    void start_receive();

    void get_feedback();

private:
    void write();
};


