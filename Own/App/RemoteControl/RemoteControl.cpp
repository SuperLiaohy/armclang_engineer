//
// Created by lhy on 2024/9/10.
//

#include "RemoteControl.h"

void RemoteControl::update() {
    auto buff = uartPlus.rx_buffer;

    rcInfo.ch1 = static_cast<int16_t>(((buff[0] | buff[1] << 8) & 0x07FF) - 1024);
    rcInfo.ch2 = static_cast<int16_t>((((buff[1] >> 3 | buff[2] << 5) & 0x07FF) - 1024));
    rcInfo.ch3 = static_cast<int16_t>((((buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF) - 1024));
    rcInfo.ch4 = static_cast<int16_t>((((buff[4] >> 1 | buff[5] << 7) & 0x07FF) - 1024));

    /* prevent remote control zero deviation */
    if (rcInfo.ch1 <= 5 && rcInfo.ch1 >= -5)
        rcInfo.ch1 = 0;
    if (rcInfo.ch2 <= 5 && rcInfo.ch2 >= -5)
        rcInfo.ch2 = 0;
    if (rcInfo.ch3 <= 5 && rcInfo.ch3 >= -5)
        rcInfo.ch3 = 0;
    if (rcInfo.ch4 <= 5 && rcInfo.ch4 >= -5)
        rcInfo.ch4 = 0;

    rcInfo.left  = ((buff[5] >> 4) & 0x000C) >> 2; //sw1   中间是3，上边是1，下边是2
    rcInfo.right = (buff[5] >> 4) & 0x0003;        //sw2

    key_board.mouse.x = static_cast<int16_t>((buff[6] | (buff[7] << 8))); // x axis
    key_board.mouse.y = static_cast<int16_t>((buff[8] | (buff[9] << 8)));
    key_board.mouse.z = static_cast<int16_t>((buff[10] | (buff[11] << 8)));

    key_board.mouse.l = buff[12];
    key_board.mouse.r = buff[13];

    key_board.kb.key_code = buff[14] | buff[15] << 8; // key borad code
    rcInfo.wheel       = static_cast<int16_t>(((buff[16] | buff[17] << 8) - 1024));

    detect.update();
}



