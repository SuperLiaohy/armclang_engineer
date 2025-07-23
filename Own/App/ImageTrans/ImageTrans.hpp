//
// Created by Administrator on 25-1-20.
//
#pragma once
#include "array"

#include "Uart/SuperUart.hpp"
#include "Key/Key.hpp"
#include "Crc/Crc.hpp"
#include "RingBuffer/RingBuffer.h"

class ImageTrans {
public:
    constexpr static uint8_t SOF = 0xA5; // 帧头
    enum class State {
        GET_FULL_FRAME,
        RC_FRAME,
        WAIT_ENOUGH_HEADER,
        WAIT_FRAME_SOF,
        WAIT_FULL_FRAME,
        ERROR_CRC8,
        ERROR_CRC16,
    };
#pragma pack(push, 1)
    struct user_custom_rx_status {
        uint8_t map_back_over: 1;
        uint8_t lock: 1;
        uint8_t pump: 1;
        uint8_t valve: 1;
        uint8_t none: 4;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct user_custom_rx_form {
        user_custom_rx_status s;
        std::array<int16_t, 6> joint;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct user_custom_tx_status {
        uint8_t enable_map_back: 1;
        uint8_t lock: 1;
        uint8_t none: 6;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct user_custom_tx_form {
        user_custom_tx_status s;
        std::array<int16_t, 6> joint;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct frame_header {
        uint8_t sof;
        uint16_t data_length;
        uint8_t seq;
        uint8_t crc8;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct frame_template {
        frame_header header;
        uint16_t cmd_id;
        uint8_t data[40];   // 实际最大为自定义控制器30字节。加上crc16应该为32字节。
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct custom_tx_frame {
        frame_header frame_head{};
        uint16_t cmd_id{};
        uint8_t data[30]{};
        uint16_t crc16{};
    };
#pragma pack(pop)


public:
    ImageTrans(UART_HandleTypeDef *huart);

    void set_map_back(const uint8_t is_able) { user_custom_tx_data.s.enable_map_back = is_able; };
    void toggle_map_back() { user_custom_tx_data.s.enable_map_back = 1 - user_custom_tx_data.s.enable_map_back; };
    [[nodiscard]] uint8_t read_map_back() const { return user_custom_tx_data.s.enable_map_back; };

    void transmit_custom_frame();
    void get_custom_feedback(const std::array<float, 6>& pos);
    void update_keyboard(KeyBoard &key_board);
    void update_rc_keyboard(uint8_t* buff, KeyBoard &key_board);
    void start_receive() {
        uartPlus.receive_dma_idle(500);
    };

    RingBuffer<200> rb;
    void set_state(ImageTrans::State s) { this->s = s; state_change = true;}
    ImageTrans::State read_state() { return s; }
    bool is_change() {
        auto is_change = state_change;
        state_change = false;
        return is_change;
    }

    user_custom_rx_form user_custom_rx_data{};
    custom_tx_frame* p_custom_tx_frame{};

    uint16_t rx_cmd_id{};//命令ID
    SuperUart uartPlus;

    Count cnt;
private:
    user_custom_tx_form user_custom_tx_data{};
    State s;
    bool state_change;
};


