//
// Created by Administrator on 24-12-6.
//
#include "Chassis/Chassis.hpp"
#include "CppTask.hpp"
#include "Interact/Interact.hpp"
#include "Matrix/Matrix.hpp"
#include "RoboArm/RoboArm.hpp"
#include "Vofa/vofa.hpp"
Vofa<3> vofa __attribute__((section(".RAM_D1")));

void ChassisTask() {
    //    float data[3] = {-46.475, 0, 255.320};
    uint32_t time = 0;
    while (1) {
        chassis.update_slope(interact.chassis.mode, interact.polarity);
        chassis.UpdatePid();
        if (++time % 2 == 0)
            chassis.send_foc();
        ChassisHeapCnt = uxTaskGetStackHighWaterMark(NULL);

        osDelay(1);
    }
}
