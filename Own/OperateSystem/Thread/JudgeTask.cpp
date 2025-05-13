//
// Created by Administrator on 25-1-21.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Judge/ui.hpp"
#include "OneStepGet/OneStepGet.hpp"
#include "atomic"

int32_t fps = 0;
std::atomic<bool> ui_reset(false);
UI::ui_control ui_list[] = {
    UI::ui_control("101", 1),
    // UI::ui_control("102", 1),
    UI::ui_control("103", 3),
    UI::ui_control("104", 3),
    UI::ui_control("105", 4),
    UI::ui_control("106", 3),
    UI::ui_control("107", 3),
    UI::ui_control("108", 3),
    UI::ui_control("109", 3),
    UI::ui_control("110", 3),
};
void JudgeTask() {
    auto int_valve_tx = []() -> int32_t {
        auto valve_tx_s = interact.sub_board.read_tx_status();
        int32_t result  = 0;
        result          = result * 10 + (0x01);
        for (int i = 6; i >= 0; --i) { result = result * 10 + ((valve_tx_s >> i) & 0x01); }
        return result;
    };
    auto int_robo_arm = []() -> int32_t {
        if (interact.robo_arm.mode == interact_dep::robo_mode::CUSTOM) { return 1; }
        return 0;
    };
    auto int_chassis = []() -> int32_t {
        if (interact.chassis.mode == interact_dep::chassis_mode::NONE) { return 0; }
        return 1;
    };
    auto int_auto = []() -> int32_t {
        if (OneStepGetControl::AUTO == OSG::mode) { return 1; }
        return 0;
    };
    while (1) {
        uint32_t time     = 0;
        ui.clear();
        osDelay(34);
        for (auto& item: ui_list) {
            item.op = UI::operation::ADD;
            item.display_num++;
        }
        ui.operate_fig("001", UI::operation::ADD, UI::layer::LAYER_4, UI::color::GREEN, 2, 24, 838,
                       UI::string_data {.font_size = 20, .data = "air:", .length = 5});
        ui.update();
        osDelay(34);

        ui.operate_fig("003", UI::operation::ADD, UI::layer::LAYER_4, UI::color::CYAN, 2, 24, 686,
                       UI::string_data {.font_size = 20, .data = "chassis:", .length = 9});
        ui.update();
        osDelay(34);
        ui.operate_fig("004", UI::operation::ADD, UI::layer::LAYER_4, UI::color::YELLOW, 2, 24, 595,
                       UI::string_data {.font_size = 20, .data = "roboarm:", .length = 9});
        ui.update();
        osDelay(34);
        ui.operate_fig("005", UI::operation::ADD, UI::layer::LAYER_4, UI::color::PINK, 2, 24, 900,
                       UI::string_data {.font_size = 20, .data = "auto:", .length = 5});
        ui.update();
        osDelay(34);

        ui.operate_fig("006", UI::operation::ADD, UI::layer::LAYER_4, UI::color::CYAN, 1, 858, 845,
                       UI::string_data {.font_size = 10, .data = "lf:", .length = 4});
        ui.update();
        osDelay(34);
        ui.operate_fig("007", UI::operation::ADD, UI::layer::LAYER_4, UI::color::CYAN, 1, 858, 790,
                       UI::string_data {.font_size = 10, .data = "lb:", .length = 4});
        ui.update();
        osDelay(34);
        ui.operate_fig("008", UI::operation::ADD, UI::layer::LAYER_4, UI::color::CYAN, 1, 1022, 845,
                       UI::string_data {.font_size = 10, .data = "rf:", .length = 4});
        ui.update();
        osDelay(34);
        ui.operate_fig("009", UI::operation::ADD, UI::layer::LAYER_4, UI::color::CYAN, 1, 1022, 790,
                       UI::string_data {.font_size = 10, .data = "rb:", .length = 4});
        ui.update();
        osDelay(34);
        ui.operate_fig("010", UI::operation::ADD, UI::layer::LAYER_4, UI::color::CYAN, 1, 930, 880,
                       UI::string_data {.font_size = 10, .data = "main:", .length = 6});
        ui.update();
        osDelay(34);

        ui.operate_fig(ui_list[0], UI::layer::LAYER_5, UI::color::GREEN, 5, 221, 838,
                       UI::int_data {.font_size = 50, .data = int_valve_tx()});
        ui.operate_fig(ui_list[1], UI::layer::LAYER_5, UI::color::CYAN, 5, 221, 686,
                       UI::int_data {.font_size = 50, .data = int_chassis()});
        ui.operate_fig(ui_list[2], UI::layer::LAYER_5, UI::color::YELLOW, 5, 221, 595,
                       UI::int_data {.font_size = 50, .data = int_robo_arm()});
        ui.operate_fig(ui_list[3], UI::layer::LAYER_5, UI::color::PINK, 5, 221, 900,
                       UI::int_data {.font_size = 50, .data = int_auto()});

        ui.update();
        osDelay(34);

        ui.operate_fig(ui_list[4], UI::layer::LAYER_4, UI::color::CYAN, 1, 858 + 50, 845,
                   UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve5});  //lf

        ui.operate_fig(ui_list[5], UI::layer::LAYER_4, UI::color::CYAN, 1, 858 + 50, 790,
                       UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve4});  //lb

        ui.operate_fig(ui_list[6], UI::layer::LAYER_4, UI::color::CYAN, 1, 1022 + 50, 845,
                       UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve1});  //rf

        ui.operate_fig(ui_list[7], UI::layer::LAYER_4, UI::color::CYAN, 1, 1022 + 50, 790,
                       UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve2});  //rb

        ui.operate_fig(ui_list[8], UI::layer::LAYER_4, UI::color::CYAN, 1, 930 + 70, 880,
                       UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve3});
        ui.update();
        osDelay(34);
        // ui.operate_fig(ui_list[1],ui_dep::layer::LAYER_0,UI::color::CYAN,5,800,400,ui_dep::int_data{.font_size = 50,.data = fps});
        // ui.operate_fig(ui_list[2],ui_dep::layer::LAYER_0,UI::color::GREEN,5,800,200,ui_dep::int_data{.font_size = 50,.data = fps});
        // ui.operate_fig(ui_list[3],ui_dep::layer::LAYER_0,UI::color::YELLOW,5,800,800,ui_dep::int_data{.font_size = 50,.data = fps});
        for (auto& item: ui_list) { item.op = UI::operation::MODIFY; }
        while (1) {

            ++time;
            if (time % 33 == 0) {
                ++fps;
                for (auto& item: ui_list) {
                    if (fps % item.display_interval == 0) { ++item.display_num; }
                }
                ui.operate_fig(ui_list[0], UI::layer::LAYER_5, UI::color::PINK, 5, 221, 838,
                               UI::int_data {.font_size = 50, .data = int_valve_tx()});

                ui.operate_fig(ui_list[1], UI::layer::LAYER_5, UI::color::CYAN, 5, 221, 686,
                               UI::int_data {.font_size = 50, .data = int_chassis()});

                ui.operate_fig(ui_list[2], UI::layer::LAYER_5, UI::color::YELLOW, 5, 221, 595,
                               UI::int_data {.font_size = 50, .data = int_robo_arm()});

                ui.operate_fig(ui_list[3], UI::layer::LAYER_5, UI::color::PINK, 5, 221, 900,
                               UI::int_data {.font_size = 50, .data = int_auto()});

                ui.operate_fig(ui_list[4], UI::layer::LAYER_4, UI::color::CYAN, 1, 858 + 50, 845,
                           UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve5});

                ui.operate_fig(ui_list[5], UI::layer::LAYER_4, UI::color::CYAN, 1, 858 + 50, 790,
                               UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve4});

                ui.operate_fig(ui_list[6], UI::layer::LAYER_4, UI::color::CYAN, 1, 1022 + 50, 845,
                               UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve1});

                ui.operate_fig(ui_list[7], UI::layer::LAYER_4, UI::color::CYAN, 1, 1022 + 50, 790,
                               UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve2});

                ui.operate_fig(ui_list[8], UI::layer::LAYER_4, UI::color::CYAN, 1, 930 + 70, 880,
                               UI::float_data {.font_size = 10, .data = interact.sub_board.custom_frame_rx.s.valve3});

                // ui.operate_fig(ui_list[1],ui_dep::layer::LAYER_0,UI::color::CYAN,5,800,400,ui_dep::int_data{.font_size = 50,.data = fps});
                // ui.operate_fig(ui_list[2],ui_dep::layer::LAYER_0,UI::color::GREEN,5,800,200,ui_dep::int_data{.font_size = 50,.data = fps});
                // ui.operate_fig(ui_list[3],ui_dep::layer::LAYER_0,UI::color::YELLOW,5,800,800,ui_dep::int_data{.f/ont_size = 50,.data = fps});
                ui.update();
            }

            if (time % 1000 == 0) { fps = 0; }
            osDelay(1);
            if (ui_reset.load()) {
                break;
            }
        }
    }
}
