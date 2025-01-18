//
// Created by Administrator on 25-1-10.
//
#include "CDC/SuperCDC.h"
#include "CppTask.h"
#include "Motor/Motor.h"
#include "Motor/lkMotor.h"
#include "Motor/M2006.h"
#include "RoboArm/RoboArm.h"
#include "Interact/Interact.h"
#include "ThreadConfig.h"
void PCTask() {
    while (1) {
        roboArm.update_relative_pos();
        xEventGroupSetBits(osEventGroup, LK_RELETIVE_GET);
        interact.transmit_relative_pos(roboArm);
        osDelay(1);
    }
}