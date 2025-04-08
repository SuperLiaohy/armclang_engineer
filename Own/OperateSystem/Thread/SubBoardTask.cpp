//
// Created by Administrator on 25-2-18.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"

void SubBoardTask() {
    interact.sub_board.set_pump(0);
    interact.sub_board.set_valve1(0);
    interact.sub_board.set_valve2(0);
    interact.sub_board.set_valve3(0);
    interact.sub_board.set_valve4(0);
    interact.sub_board.set_valve5(0);
    for (;;) {
        interact.sub_board.transmit();
        osDelay(100);
    }
}