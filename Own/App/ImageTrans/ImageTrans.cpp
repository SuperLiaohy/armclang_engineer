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

bool ImageTrans::update() {
//    kb.mouse = 0;
    auto buff = uartPlus.rx_buffer;
    return  (buff[0] == image_trans_dep::SOF && verify_crc8_check_sum(buff, 5));
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
