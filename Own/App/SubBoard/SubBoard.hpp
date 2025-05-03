//
// Created by Administrator on 2025/2/28.
//
#pragma once


#include "Uart/SuperUart.hpp"

namespace sub_board_dep {
    typedef struct {
        uint8_t sof;
    } __attribute__((packed)) frame_header;


    typedef struct {
        uint8_t valve1: 1;
        uint8_t valve2: 1;
        uint8_t valve3: 1;
        uint8_t valve4: 1;
        uint8_t valve5: 1;
        uint8_t valve6: 1;
        uint8_t none: 1;
        uint8_t pump: 1;
    } __attribute__((packed)) tx_status;
    typedef struct {
        tx_status s;
    } __attribute__((packed)) custom_tx_frame;
    typedef struct {
        frame_header frame_head;
        uint8_t cmd_id;
        uint8_t data[8 - sizeof(frame_header) - sizeof(uint8_t) - sizeof(uint8_t)];
        uint8_t crc8;
    } __attribute__((packed)) trans_frame;


    typedef struct {
        uint8_t valve1: 1;
        uint8_t valve2: 1;
        uint8_t valve3: 1;
        uint8_t valve4: 1;
        uint8_t valve5: 1;
        uint8_t none: 3;
    } __attribute__((packed)) rx_status;
    typedef struct {
        rx_status s;
//    float data[8];
    } __attribute__((packed)) custom_rx_frame;
    typedef struct {
        frame_header frame_head;
        uint8_t cmd_id;
        uint8_t data[8 - sizeof(frame_header) - sizeof(uint8_t) - sizeof(uint8_t)];
        uint8_t crc8;
        uint8_t none[8];
    } __attribute__((packed)) receive_frame;


} // sub_board_dep


class SubBoard {
public:
    using custom_rx_frame = sub_board_dep::custom_rx_frame;
    using receive_frame = sub_board_dep::receive_frame;
    using custom_tx_frame = sub_board_dep::custom_tx_frame;
    using trans_frame = sub_board_dep::trans_frame;

    SubBoard(UART_HandleTypeDef *uart) : uartPlus(uart, sizeof(receive_frame), sizeof(trans_frame)) {
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

    [[nodiscard]] bool read_valve1() const { return custom_frame_rx.s.valve1; };

    [[nodiscard]] bool read_valve2() const { return custom_frame_rx.s.valve2; };

    [[nodiscard]] bool read_valve3() const { return custom_frame_rx.s.valve3; };

    [[nodiscard]] bool read_valve4() const { return custom_frame_rx.s.valve4; };

    [[nodiscard]] bool read_valve5() const { return custom_frame_rx.s.valve5; };

    void transmit();

    void start_receive();

    void get_feedback();

private:
    void write();
};


