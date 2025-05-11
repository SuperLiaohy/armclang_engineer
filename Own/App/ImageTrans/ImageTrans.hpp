//
// Created by Administrator on 25-1-20.
//
#pragma once
#include "array"

#include "Uart/SuperUart.hpp"
#include "Key/Key.hpp"
#include "Crc/Crc.hpp"
#include "RoboArm/roboarm_dep.hpp"



namespace image_trans_dep {
    constexpr uint8_t SOF = 0xA5; // 帧头

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
    struct user_custom_rx_data {
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
    struct user_custom_tx_data {
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
    struct custom_tx_frame {
        frame_header frame_head{};
        uint16_t cmd_id{};
        uint8_t data[30]{};
        uint16_t crc16{};
    };
#pragma pack(pop)
}

class ImageTrans {
public:
    ImageTrans(UART_HandleTypeDef *huart) : uartPlus(huart, 100, 100) {
        p_custom_tx_frame = reinterpret_cast<image_trans_dep::custom_tx_frame*>(uartPlus.tx_buffer);
        p_custom_tx_frame->frame_head = {0xA5, 30, 0, 0};
        crc::append_crc8_check_sum(reinterpret_cast<uint8_t*>(&p_custom_tx_frame->frame_head), sizeof(image_trans_dep::frame_header));
        p_custom_tx_frame->cmd_id = 0x309;
    }

    void set_map_back(const uint8_t is_able) { user_custom_tx_data.s.enable_map_back = is_able; };
    void toggle_map_back() { user_custom_tx_data.s.enable_map_back = 1 - user_custom_tx_data.s.enable_map_back; };
    [[nodiscard]] uint8_t read_map_back() const { return user_custom_tx_data.s.enable_map_back; };

    void transmit_custom_frame();
    void get_custom_feedback(const std::array<float, 6>& pos);
    void update_keyboard(KeyBoard &key_board);

    image_trans_dep::user_custom_rx_data user_custom_rx_data{};
    image_trans_dep::custom_tx_frame* p_custom_tx_frame{};
    image_trans_dep::user_custom_tx_status last_s;
    uint16_t rx_cmd_id{};//命令ID
    SuperUart uartPlus;

    Count cnt;
private:
    image_trans_dep::user_custom_tx_data user_custom_tx_data{};
};


