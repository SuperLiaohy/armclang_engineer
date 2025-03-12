//
// Created by Administrator on 25-2-16.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"

void KbTask() {

    while (1) {
        if (interact.kb != interact_dep::kb_state::DISABLE) {
            KeyBoardUpdate(interact.keyList, interact.key_board);
        }
        osDelay(1);
    }
}