//
// Created by Administrator on 2025/2/28.
//

#include "SubBoard.hpp"

namespace sub_board_dep {
} // sub_board_dep


void SubBoard::transmit() {
    write();
    uartPlus.transmit(sizeof(trans_frame));
}

void SubBoard::start_receive() {
    uartPlus.read_idle(sizeof(receive_frame));
}

void SubBoard::write() {
    memcpy(tx_frame->data, &custom_frame_tx, sizeof(custom_frame_tx));
}

void SubBoard::get_feedback() {
    if (rx_frame->frame_head.sof == 0xA5 ) {
        switch (rx_frame->cmd_id) {
            case 0x55:
                memcpy(&custom_frame_rx, rx_frame->data, sizeof(custom_frame_rx));
                break;
            default:
                break;

        }
    }
}
