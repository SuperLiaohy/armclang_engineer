//
// Created by Administrator on 25-2-18.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"

void SubBoardTask() {
    for (;;) {
        interact.sub_board.transmit();
        osDelay(100);
    }
}