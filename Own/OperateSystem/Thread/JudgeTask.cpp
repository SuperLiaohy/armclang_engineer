//
// Created by Administrator on 25-1-21.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Judge/ui.hpp"

UI::VarUiItem ui_list[] = {
    UI::UiItem<ui_dep::line>(reinterpret_cast<const uint8_t*>("23"), 0),
    UI::UiItem<ui_dep::line>(reinterpret_cast<const uint8_t*>("23"), 0),
    UI::UiItem<ui_dep::line>(reinterpret_cast<const uint8_t*>("23"), 0),
    UI::UiItem<ui_dep::line>(reinterpret_cast<const uint8_t*>("23"), 0),
    UI::UiItem<ui_dep::line>(reinterpret_cast<const uint8_t*>("23"), 0),
};

void JudgeTask() {
    uint32_t time = 0;
    int32_t fps   = 0;
    // constexpr float a[10]={};
    ui.update();

    while (1) {
        ++time;
        if (time % 33 == 0) {
            ++fps;
            for (auto& item: ui_list) {
                std::visit(
                    [&fps](auto& arg) {
                        if (fps % arg.control.display_interval == 0) ++arg.control.display_num;
                        ui.operate_fig(arg);
                    },
                    item);
            }
            ui.update();
        }
        if (time % 1000 == 0) { fps = 0; }
        osDelay(1);
    }
}
