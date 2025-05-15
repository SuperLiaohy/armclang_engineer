//
// Created by Administrator on 25-1-21.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"

uint16_t cmd_id = 0;
uint16_t cmd    = 0;

void ImageTransTask() {
    interact.image_trans.set_map_back(0);
    while (1) {
        auto now = xTaskGetTickCount();
        // if (interact.robo_arm.mode == interact_dep::robo_mode::CUSTOM) {
        if (interact.robo_arm.mode == interact_dep::robo_mode::CUSTOM) {
            interact.image_trans.set_map_back(0);
        } else {
            interact.image_trans.set_map_back(1);
        }
        interact.image_trans.get_custom_feedback(roboArm.relative_pos);
        interact.image_trans.transmit_custom_frame();
        // }
        osDelayUntil(&now, 100);
    }
}