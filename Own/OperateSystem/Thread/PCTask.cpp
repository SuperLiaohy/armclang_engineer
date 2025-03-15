//
// Created by Administrator on 25-1-10.
//
#include "CDC/SuperCDC.hpp"
#include "CppTask.hpp"
#include "Motor/Motor.hpp"
#include "Motor/lkMotor.hpp"
#include "Motor/M2006.hpp"
#include "RoboArm/RoboArm.hpp"
#include "Interact/Interact.hpp"
#include "ThreadConfig.h"

extern uint8_t cmd;
void PCTask() {
    uint32_t cnt = 0;
    while (1) {

        if (cnt++%100==0) {
            roboArm.update_relative_pos();
            xEventGroupSetBits(osEventGroup, LK_RECEIVE_GET);
            interact.transmit_relative_pos(roboArm);
        }
        osDelay(1);
    }
}

