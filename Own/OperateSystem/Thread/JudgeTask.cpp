//
// Created by Administrator on 25-1-21.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Judge/ui.hpp"
void JudgeTask() {
    uint32_t time = 0;
    int32_t fps   = 0;

    ui.operate_fig(reinterpret_cast<const uint8_t*>("23"), UI::operation::ADD, ui_dep::layer::LAYER_5,
                   ui_dep::color::PINK, 3, 500, 500, ui_dep::circle {.radius = 250});
    ui.operate_fig(reinterpret_cast<const uint8_t*>("24"), UI::operation::ADD, ui_dep::layer::LAYER_5,
                   ui_dep::color::PINK, 3, 600, 600,
                   ui_dep::arc {.start_angle = 0, .end_angle = 11, .radius_x = 720, .radius_y = 360});
    ui.operate_fig(reinterpret_cast<const uint8_t*>("25"), UI::operation::ADD, ui_dep::layer::LAYER_5,
                   ui_dep::color::PINK, 3, 800, 800, ui_dep::rect {.opposed_x = 250, .opposed_y = 250});
    ui.operate_fig(reinterpret_cast<const uint8_t*>("26"), UI::operation::ADD, ui_dep::layer::LAYER_5,
                   ui_dep::color::PINK, 3, 500, 500, ui_dep::ellipse {.radius_x = 250, .radius_y = 150});
    ui.operate_fig(reinterpret_cast<const uint8_t*>("27"), UI::operation::ADD, ui_dep::layer::LAYER_5,
                   ui_dep::color::PINK, 3, 800, 200, ui_dep::float_data {.font_size = 30, .data = 100});
    ui.operate_fig(reinterpret_cast<const uint8_t*>("28"), UI::operation::ADD, ui_dep::layer::LAYER_5,
                   ui_dep::color::PINK, 3, 500, 500, ui_dep::line {.end_x = 600, .end_y = 600});
    ui.operate_fig(reinterpret_cast<const uint8_t*>("29"), UI::operation::ADD, ui_dep::layer::LAYER_5,
                   ui_dep::color::GREEN, 3, 900, 900, ui_dep::int_data {.font_size = 25, .data = fps});
    ui.update();

    while (1) {
        ++time;
        if (time % 33 == 0) {
            ++fps;

            if (ui.type != UI::types::DELETE) {
                ui.operate_fig(reinterpret_cast<const uint8_t*>("123"), UI::operation::ADD, ui_dep::layer::LAYER_0, ui_dep::color::BLACK,
                3, 800, 800,
                ui_dep::string_data<5>{.font_size=30,.data={"666"}});
            }

            if (ui.type != UI::types::DELETE && ui.type != UI::types::STRING) {
                ui.operate_fig(reinterpret_cast<const uint8_t*>("23"), UI::operation::MODIFY, ui_dep::layer::LAYER_5,
                               ui_dep::color::PINK, 3, 500, 500, ui_dep::circle {.radius = 250});
                ui.operate_fig(reinterpret_cast<const uint8_t*>("24"), UI::operation::MODIFY, ui_dep::layer::LAYER_5,
                               ui_dep::color::PINK, 3, 600, 600,
                               ui_dep::arc {.start_angle = 0, .end_angle = 11, .radius_x = 720, .radius_y = 360});
                ui.operate_fig(reinterpret_cast<const uint8_t*>("25"), UI::operation::MODIFY, ui_dep::layer::LAYER_5,
                               ui_dep::color::PINK, 3, 800, 800, ui_dep::rect {.opposed_x = 250, .opposed_y = 250});
                ui.operate_fig(reinterpret_cast<const uint8_t*>("26"), UI::operation::MODIFY, ui_dep::layer::LAYER_5,
                               ui_dep::color::PINK, 3, 500, 500, ui_dep::ellipse {.radius_x = 250, .radius_y = 150});
                ui.operate_fig(reinterpret_cast<const uint8_t*>("27"), UI::operation::MODIFY, ui_dep::layer::LAYER_5,
                               ui_dep::color::PINK, 3, 800, 200, ui_dep::float_data {.font_size = 30, .data = 100});
                ui.operate_fig(reinterpret_cast<const uint8_t*>("28"), UI::operation::MODIFY, ui_dep::layer::LAYER_5,
                               ui_dep::color::PINK, 3, 500, 500, ui_dep::line {.end_x = 600, .end_y = 600});
                ui.operate_fig(reinterpret_cast<const uint8_t*>("29"), UI::operation::MODIFY, ui_dep::layer::LAYER_5,
                               ui_dep::color::GREEN, 3, 900, 900, ui_dep::int_data {.font_size = 25, .data = fps});
            }

            ui.update();
            // do something
        }
        if (time % 1000 == 0) { fps = 0; }
        osDelay(1);
    }
}
