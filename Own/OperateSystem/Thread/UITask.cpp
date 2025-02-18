//
// Created by Administrator on 25-2-18.
//
#include "CppTask.h"
#include "Judge/ui.h"
void UITask() {
    uint32_t time = 0;
    for (;;) {
        ++time;

        if (time % 32 == 0) {
            ui.operate_str(reinterpret_cast<const uint8_t*>("123"), UI::operation::ADD,
                           1, UI::color::SELF_COLOR, 100, 9, 8, 0, 0, reinterpret_cast<const uint8_t*>("engineer"));
            ui.add_frame_header();
            HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<const uint8_t*>(&ui.frame), ui.len + 15);
            ui.clear();
            // do something
        }
        osDelay(1);
    }
}