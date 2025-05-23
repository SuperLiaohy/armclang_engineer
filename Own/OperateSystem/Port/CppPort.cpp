//
// Created by lhy on 2024/9/19.
//
#include "CDC/SuperCDC.hpp"
#include "CppPort.h"
#include "GPIO/SuperGPIO.hpp"
#include "Interact/Interact.hpp"
#include "W25Q64/W25Q64.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

extern uint32_t flash_data;


void start() {
    interact.remote_control.start();
}

extern "C" void start_c(void){
    start();
}
extern "C" void im_start_c(void){
    interact.image_trans.uartPlus.receive_dma_idle(100);
}



extern "C" void hard_fault_reset() {
    if (interact.remote_control.rcInfo.right == 2)
        while (1) {
            if (interact.remote_control.rcInfo.right == 3)
                HAL_NVIC_SystemReset();
        }
}
