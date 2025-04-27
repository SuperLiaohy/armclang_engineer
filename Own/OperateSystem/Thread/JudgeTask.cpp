//
// Created by Administrator on 25-1-21.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Judge/ui.hpp"
void JudgeTask() {
    uint32_t time = 0;
    while (1) {
        ++time;
        if (time % 100 == 0) {
            // ui.operate_str(reinterpret_cast<const uint8_t*>("123"), UI::operation::ADD,
            //                1, UI::color::SELF_COLOR, 20, 9, 2, 200, 200, reinterpret_cast<const uint8_t*>("engineer"));

            if (ui.type != UI::types::DELETE) {
//                ui.operate_fig(reinterpret_cast<const uint8_t*>("123"), UI::operation::ADD, UI::graphic::STR, ui_dep::layer::LAYER_0, ui_dep::color::BLACK,
//                               3, 500, 500,
//                               ui_dep::string_data<5>{.font_size=30,.data={"666"}});

            }

            if (ui.type != UI::types::DELETE && ui.type != UI::types::STRING) {
                ui.operate_fig(reinterpret_cast<const uint8_t*>("23"), UI::operation::ADD, UI::graphic::CIRCLE, ui_dep::layer::LAYER_5, ui_dep::color::PINK,
                               3, 500, 500,
                               ui_dep::circle{.radius = 50});

            }

            ui.update();
            // do something
        }
        osDelay(1);
    }
}
