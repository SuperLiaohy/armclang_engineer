//
// Created by Administrator on 25-2-12.
//
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
void TransmitTask() {
        interact.image_trans.set_map_back(1);

    while (1) {
        auto now = xTaskGetTickCount();
//        interact.image_trans.set_map_back(1);
        if (interact.path == interact_dep::path::IMAGE_TRANSMIT) {
            interact.image_trans.get_angle(roboArm.real_relative_pos);
            interact.image_trans.transmit();
        }
        osDelayUntil(&now, 100);
    }
}