//
// Created by Administrator on 25-1-20.
//

#include "ImageTrans.hpp"

#include "Interact/Interact.hpp"
#include "Interact/Interact_dep.hpp"

// __attribute__((section(".RAM_D1"))) static uint8_t image_trans_rx_buffer[500];
// __attribute__((section(".RAM_D1"))) static uint8_t image_trans_tx_buffer[500];

ImageTrans::ImageTrans(UART_HandleTypeDef* huart) : uartPlus(huart, 500, 100) {
    // uartPlus.rx_buffer = image_trans_rx_buffer;
    // uartPlus.rx_buffer = image_trans_tx_buffer;
    p_custom_tx_frame = reinterpret_cast<custom_tx_frame*>(uartPlus.tx_buffer);
    p_custom_tx_frame->frame_head = {0xA5, 30, 0, 0};
    crc::append_crc8_check_sum(reinterpret_cast<uint8_t*>(&p_custom_tx_frame->frame_head), sizeof(frame_header));
    p_custom_tx_frame->cmd_id = 0x309;
}


void ImageTrans::update_keyboard(KeyBoard& key_board) {
    const auto buff = uartPlus.rx_buffer;
    key_board.mouse.x = static_cast<int16_t>((buff[7] | (buff[8] << 8))); // x axis
    key_board.mouse.y = static_cast<int16_t>((buff[9] | (buff[10] << 8)));
    key_board.mouse.z = static_cast<int16_t>((buff[11] | (buff[12] << 8)));

    key_board.mouse.l = buff[13];
    key_board.mouse.r = buff[14];

    key_board.kb.key_code = buff[15] | buff[16] << 8; // key borad code
}

void ImageTrans::transmit_custom_frame() {
    memcpy(p_custom_tx_frame->data, &user_custom_tx_data, sizeof(user_custom_tx_data));
    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(p_custom_tx_frame), sizeof(custom_tx_frame));
    uartPlus.transmit_dma_pdata(uartPlus.tx_buffer, sizeof(custom_tx_frame));
}

void ImageTrans::get_custom_feedback(const std::array<float, 6>& angle) {
    user_custom_tx_data.joint[0] = -angle[0] * scale(360, 4096);
    user_custom_tx_data.joint[1] = angle[1] * scale(360, 4096);
    user_custom_tx_data.joint[2] = angle[2] * scale(360, 4096);
    user_custom_tx_data.joint[3] = -angle[3] * scale(360, 4096);
    user_custom_tx_data.joint[4] = angle[4] * scale(360, 4096);
    user_custom_tx_data.joint[5] = angle[5] * scale(360, 4096);
}
