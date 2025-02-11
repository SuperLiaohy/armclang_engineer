//
// Created by Administrator on 25-1-21.
//
#include "CppTask.h"
#include "Interact/Interact.h"

uint16_t cmd_id = 0;
uint16_t cmd = 0;

void ImageTransTask() {
    while (1) {
        using namespace crc;

        cmd = 0;
        vTaskSuspend(NULL);
        cmd = 1;
        auto data = interact.image_trans.uartPlus.rx_buffer;
        uint16_t len = (data[2] << 8 | data[1]);
        interact.image_trans.rx_cmd_id = data[6] << 8 | data[5];
        if (verify_crc16_check_sum(data, len + 9)) {
            switch (interact.image_trans.rx_cmd_id) {
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