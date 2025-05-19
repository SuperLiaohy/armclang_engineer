//
// Created by Administrator on 25-2-18.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"

void SubBoardTask() {
    interact.sub_board.set_pump(0);
    interact.sub_board.set_rf_valve(0);
    interact.sub_board.set_rb_valve(0);
    interact.sub_board.set_main_valve(0);
    interact.sub_board.set_lb_valve(0);
    interact.sub_board.set_lf_valve(0);
    interact.sub_board.set_chassis_valve(0);
    for (;;) {
        interact.sub_board.transmit();
        osDelay(14);
    }
}