//
// Created by lhy on 2024/9/19.
//
#include "CDC/SuperCDC.h"
#include "CppPort.h"
#include "GPIO/SuperGPIO.h"
#include "Interact/interact.h"
#include "W25Q64/W25Q64.h"

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

void flash_write() {
    uint32_t data = 4399;
    w25q64.write_page(0, reinterpret_cast<uint8_t*>(&data), 4);
}
extern "C" void flash_write_c(void){
    flash_write();
}

void flash_init() {
    w25q64.init();
}
void flash_init_c() {
    flash_init();
}

void flash_read() {
    w25q64.raed_buffer(0, reinterpret_cast<uint8_t*>(&flash_data), 4);
}
extern "C" void flash_read_c(void){
    flash_read();
}


extern "C" void hard_fault_reset() {
    if (interact.remote_control.rcInfo.right == 2)
        while (1) {
            if (interact.remote_control.rcInfo.right == 3)
                HAL_NVIC_SystemReset();
        }
}
