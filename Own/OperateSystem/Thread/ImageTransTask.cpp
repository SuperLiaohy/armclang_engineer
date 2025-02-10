//
// Created by Administrator on 25-1-21.
//
#include "CppTask.h"
#include "Interact/Interact.h"


void ImageTransTask() {
    while (1) {
        using namespace crc;
        interact.image_trans.uartPlus.read_idle(50);
        vTaskSuspend(NULL);
        auto data = interact.image_trans.uartPlus.rx_buffer;
        uint16_t len = (data[1]<<8|data[2]);
        if (verify_crc16_check_sum(data, len + 9)) {
            switch (data[5]<<8|data[6]) {
                case 0x302:
                    interact.receive_custom(&data[7]);
                    break;
                case 0x304:
                    break;
                case 0x306:
                    break;
                case 0x309:
                    break;
                default:
                    break;

            }
        }
    }
}