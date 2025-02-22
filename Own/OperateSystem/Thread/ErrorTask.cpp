//
// Created by liaohy on 24-11-15.
//
#include "CppTask.h"
#include "RGBLED/RGBLED.h"
#include "RemoteControl/RemoteControl.h"

#ifdef __cplusplus
extern "C" {
#endif

extern osThreadId ERROR_TASKHandle;

#ifdef __cplusplus
}
#endif

#include "CAN/SuperCan.h"
#include "Interact/interact.h"
#include "Motor/M2006.h"
#include "Motor/Motor.h"
#include "Motor/lkMotor.h"
#include "RoboArm/RoboArm.h"
extern uint8_t re_flag;
void ErrorTask() {
    osThreadSuspend(ERROR_TASKHandle);
//    if (re_flag == 0) {
//        vTaskDelete(ARM_INIT_TASKHandle);
//    }
    uint8_t red = 0;
    while (1) {

        roboArm.close();
        canPlus1.write(0, 0, 0, 0);
        canPlus1.send(Motor<M2006>::foc.TX_LOW_ID);
        canPlus2.write(0, 0, 0, 0);
        canPlus2.send(Motor<M3508>::foc.TX_LOW_ID);
        Led.SetColor(red * 255, 0, 0);
        red = 1 - red;

//        chassiss.stop();
        if (interact.remote_control.rcInfo.right == 3) {
            __set_FAULTMASK(1);
            HAL_NVIC_SystemReset();
        }
        HAL_Delay(14);
        ErrorHeapCnt = uxTaskGetStackHighWaterMark(NULL);
    }
}
