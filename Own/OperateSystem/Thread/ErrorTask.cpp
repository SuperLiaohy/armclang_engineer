//
// Created by liaohy on 24-11-15.
//
#include "RemoteControl/RemoteControl.h"
#include "RGBLED/RGBLED.h"
#include "CppTask.h"

#ifdef __cplusplus
extern "C" {
#endif

extern osThreadId ERROR_TASKHandle;

#ifdef __cplusplus
}
#endif

#include "CAN/SuperCan.h"
#include "Motor/Motor.h"
#include "Motor/M2006.h"
#include "Motor/lkMotor.h"
#include "RoboArm/RoboArm.h"
#include "Interact/interact.h"
void ErrorTask() {
    osThreadSuspend(ERROR_TASKHandle);
    uint8_t red = 0;
    while (1) {
        // chassiss.stop();
        if (interact.remote_control.rcInfo.right == 3) {
            __set_FAULTMASK(1);
            HAL_NVIC_SystemReset();
        }
        red = 1 - red;
        canPlus1.write(0, 0, 0, 0);
        canPlus1.send(Motor<M2006>::foc.TX_LOW_ID);
        canPlus2.write(0, 0, 0, 0);
        canPlus2.send(Motor<M2006>::foc.TX_LOW_ID);
//        roboArm.disable();
        roboArm.close();

        Led.SetColor_Hal(red * 255,0,0);
//        HAL_UART_Transmit(&huart10,(uint8_t*)"hello world\n", 12, 0xffff);
        HAL_Delay(50);
        ErrorHeapCnt = uxTaskGetStackHighWaterMark(NULL);
    }
}
