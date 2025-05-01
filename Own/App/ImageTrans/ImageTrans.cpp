//
// Created by Administrator on 25-1-20.
//

#include "ImageTrans.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "stdlib.h"
#include "cmsis_os.h"
extern osThreadId IMAGEATRANS_TASHandle;
#ifdef __cplusplus
};
#endif


using namespace crc;

void ImageTrans::update(KeyBoard& key_board) {
    const auto buff = uartPlus.rx_buffer;
    key_board.mouse.x = static_cast<int16_t>((buff[7] | (buff[8] << 8))); // x axis
    key_board.mouse.y = static_cast<int16_t>((buff[9] | (buff[10] << 8)));
    key_board.mouse.z = static_cast<int16_t>((buff[11] | (buff[12] << 8)));

    key_board.mouse.l = buff[13];
    key_board.mouse.r = buff[14];

    key_board.kb.key_code = buff[15] | buff[16] << 8; // key borad code
}

void ImageTrans::transmit() {
    memcpy(tx_frame.data, &custom_tx_frame, sizeof(custom_tx_frame));
    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(&tx_frame), sizeof(tx_frame));
    memcpy(uartPlus.tx_buffer, &tx_frame, sizeof(tx_frame));
    uartPlus.transmit_dma_pdata(uartPlus.tx_buffer, sizeof(tx_frame));
}

void ImageTrans::get_angle(const std::array<float, 6>& angle) {
    custom_tx_frame.joint[0] = angle[0] * scale(360, 4096);
    custom_tx_frame.joint[1] = angle[1] * scale(360, 4096);
    custom_tx_frame.joint[2] = angle[2] * scale(360, 4096);
    custom_tx_frame.joint[3] = angle[3] * scale(360, 4096);
    custom_tx_frame.joint[4] = angle[4] * scale(360, 4096);
    custom_tx_frame.joint[5] = angle[5] * scale(360, 4096);
}
