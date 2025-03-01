//
// Created by Administrator on 25-1-21.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"

void JudgeTask() {
    while (1) {
//        xEventGroupWaitBits(osEventGroup, JUDGE_EVENT, pdFALSE, pdTRUE, portMAX_DELAY);
//        referee_system.update();
        interact.sub_board.transmit();

        osDelay(100);
    }
}