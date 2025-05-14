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
#include "Motor/M2006Diff.hpp"
#include "Motor/Motor.hpp"
#include "Buzzer/Buzzer.hpp"
#include "RoboArm/RoboArm.hpp"
#include "WDG/SuperIWDG.hpp"

extern uint8_t re_flag;
void ErrorTask() {
    osThreadSuspend(ERROR_TASKHandle);
    // if (re_flag == 0) {
    //     vTaskDelete(ARM_INIT_TASKHandle);
    // }
    uint32_t time = 0;
    uint8_t red = 0;
    while (1) {
        roboArm.close();
        buzzer.StartMusic<8, HAL_Delay>(Buzzer::error_music);
        canPlus1.transmit(M2006Diff::foc.TX_LOW_ID, 0, 0, 0, 0);
        canPlus2.transmit(M3508::foc.TX_LOW_ID, 0, 0, 0, 0);
        canPlus3.transmit(M3508::foc.TX_LOW_ID, 0, 0, 0, 0);
        interact.sub_board.set_pump(0);
        interact.sub_board.set_valve1(0);
        interact.sub_board.set_valve2(0);
        interact.sub_board.set_valve3(0);
        interact.sub_board.set_valve4(0);
        interact.sub_board.set_valve5(0);
        interact.sub_board.set_valve6(0);
        interact.sub_board.transmit();
        Led.SetColor(red * 255, 0, 0);
        ++time;
        if (time % 3 == 0) { red = 1 - red; }

        interact.remote_control.detect.JudgeLost();


        if (interact.remote_control.rcInfo.right != static_cast<uint8_t>(RemoteControl::lever::lower)
            || interact.remote_control.rcInfo.left != static_cast<uint8_t>(RemoteControl::lever::lower)) {
            __set_FAULTMASK(1);
            HAL_NVIC_SystemReset();
        }
        SuperIWDG::GotInstance().give();
        HAL_Delay(30);
        ErrorHeapCnt = uxTaskGetStackHighWaterMark(NULL);
    }
}
