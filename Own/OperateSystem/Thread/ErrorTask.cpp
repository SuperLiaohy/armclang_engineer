//
// Created by liaohy on 24-11-15.
//
#include "CppTask.hpp"
#include "RGBLED/RGBLED.hpp"
#include "RemoteControl/RemoteControl.hpp"

#ifdef __cplusplus
extern "C" {
#endif

extern osThreadId ERROR_TASKHandle;

#ifdef __cplusplus
}
#endif
#include "CAN/SuperCan.hpp"
#include "Interact/Interact.hpp"
#include "Motor/M2006.hpp"
#include "Motor/Motor.hpp"
#include "Motor/lkMotor.hpp"
#include "RoboArm/RoboArm.hpp"
#include "WDG/SuperIWDG.hpp"
extern uint8_t re_flag;
void ErrorTask() {
    osThreadSuspend(ERROR_TASKHandle);
    // if (re_flag == 0) {
    //     vTaskDelete(ARM_INIT_TASKHandle);
    // }
    uint32_t time;
    uint8_t red = 0;
    while (1) {
        roboArm.close();
        buzzer.StartMusic<8, Buzzer::delay_pl::HAL>(Buzzer::error_music);
        canPlus1.transmit(M2006::foc.TX_LOW_ID, 0, 0, 0, 0);
        canPlus2.transmit(M3508::foc.TX_LOW_ID, 0, 0, 0, 0);
        Led.SetColor(red * 255, 0, 0);
        ++time;
        if (time % 3 == 0) {
            red = 1 - red;
        }

        //        chassiss.stop();
        if (interact.remote_control.rcInfo.right == 3) {
            __set_FAULTMASK(1);
            HAL_NVIC_SystemReset();
        }
        SuperIWDG::GotInstance().give();
        HAL_Delay(30);
        ErrorHeapCnt = uxTaskGetStackHighWaterMark(NULL);
    }
}
