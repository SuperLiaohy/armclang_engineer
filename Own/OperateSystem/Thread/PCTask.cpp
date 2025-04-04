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

void PCTask() {
    uint32_t cnt = 0;
    interact.pc.transmit_data.head = interact.pc.head;
    interact.pc.transmit_data.tail = interact.pc.tail;
    interact.pc.transmit_data.cmd = 0xA5;
    while (1) {
        if (cnt++%10==0) {
            roboArm.update_relative_pos();
            xEventGroupSetBits(osEventGroup, LK_RECEIVE_GET);
            interact.transmit_relative_pos(roboArm.relative_pos);
        }
        osDelay(1);
    }
}

