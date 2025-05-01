//
// Created by Administrator on 25-1-21.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Judge/ui.hpp"

int32_t fps             = 0;

UI::ui_control ui_list[] = {
    UI::ui_control(reinterpret_cast<const uint8_t*>("123"),5),
    UI::ui_control(reinterpret_cast<const uint8_t*>("223"),6),
    UI::ui_control(reinterpret_cast<const uint8_t*>("323"),7),
    UI::ui_control(reinterpret_cast<const uint8_t*>("423"),1),
};

void JudgeTask() {
    uint32_t time = 0;

    ui.operate_fig(ui_list[0],ui_dep::layer::LAYER_0,UI::color::PINK,5,800,600,ui_dep::int_data{.font_size = 50,.data = fps});
    ui.operate_fig(ui_list[1],ui_dep::layer::LAYER_0,UI::color::CYAN,5,800,400,ui_dep::int_data{.font_size = 50,.data = fps});
    ui.operate_fig(ui_list[2],ui_dep::layer::LAYER_0,UI::color::GREEN,5,800,200,ui_dep::int_data{.font_size = 50,.data = fps});
    ui.operate_fig(ui_list[3],ui_dep::layer::LAYER_0,UI::color::YELLOW,5,800,800,ui_dep::int_data{.font_size = 50,.data = fps});
    ui.update();
    for (auto& item : ui_list) {
        item.op = UI::operation::MODIFY;
    }
    while (1) {
        ++time;
        if (time % 33 == 0) {
            ++fps;
            for (auto& item : ui_list) {
                if (fps % item.display_interval == 0) {
                    ++item.display_num;
                }
            }
            ui.operate_fig(ui_list[0],ui_dep::layer::LAYER_0,UI::color::PINK,5,800,600,ui_dep::int_data{.font_size = 50,.data = fps});
            ui.operate_fig(ui_list[1],ui_dep::layer::LAYER_0,UI::color::CYAN,5,800,400,ui_dep::int_data{.font_size = 50,.data = fps});
            ui.operate_fig(ui_list[2],ui_dep::layer::LAYER_0,UI::color::GREEN,5,800,200,ui_dep::int_data{.font_size = 50,.data = fps});
            ui.operate_fig(ui_list[3],ui_dep::layer::LAYER_0,UI::color::YELLOW,5,800,800,ui_dep::int_data{.font_size = 50,.data = fps});
            ui.update();
        }
        if (time % 1000 == 0) { fps = 0; }
        osDelay(1);
    }
}
