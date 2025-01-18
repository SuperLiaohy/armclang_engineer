//
// Created by Administrator on 24-12-6.
//
#include "Chassis/Chassis.h"
#include "CppTask.h"
#include "Interact/Interact.h"
#include "Matrix/Matrix.h"
#include "RoboArm/RoboArm.h"
#include "Vofa/vofa.h"
Vofa<3> vofa __attribute__((section(".RAM_D1")));

void ChassisTask() {
    //    float data[3] = {-46.475, 0, 255.320};
    while (1) {
        chassis.update_slope();
        chassis.UpdatePid();
        chassis.send_foc();
        ChassisHeapCnt = uxTaskGetStackHighWaterMark(NULL);

        osDelay(1);
    }
}
