//
// Created by Administrator on 25-1-20.
//

#include "ImageTrans.hpp"

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
    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(p_custom_tx_frame), sizeof(image_trans_dep::custom_tx_frame));
    uartPlus.transmit_dma_pdata(uartPlus.tx_buffer, sizeof(image_trans_dep::custom_tx_frame));
}

void ImageTrans::get_custom_feedback(const std::array<float, 6>& angle) {
    user_custom_tx_data.joint[0] = angle[0] * scale(360, 4096);
    user_custom_tx_data.joint[1] = angle[1] * scale(360, 4096);
    user_custom_tx_data.joint[2] = angle[2] * scale(360, 4096);
    user_custom_tx_data.joint[3] = angle[3] * scale(360, 4096);
    user_custom_tx_data.joint[4] = angle[4] * scale(360, 4096);
    user_custom_tx_data.joint[5] = angle[5] * scale(360, 4096);
}
