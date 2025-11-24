//
// Created by liaohy on 24-11-15.
//

#include "Buzzer/Buzzer.hpp"
#include "CAN/SuperCan.hpp"
#include "Detect/Detect.hpp"
#include "Interact/Interact.hpp"
#include "Pump/Pump.hpp"
#include "ThreadConfig.h"
#include <DWT/SuperDWT.hpp>

#include "SysTick/SysTick.hpp"


#ifdef __cplusplus
extern "C" {
#endif
extern osThreadId ERROR_TASKHandle;
#include "usb_device.h"
#ifdef __cplusplus
}
#endif
#include "WDG/SuperIWDG.hpp"

volatile uint32_t ada = 0;
std::atomic<bool> rc_ready(false);

extern "C" void StartDefaultTask(void const*argument) {
    ada = Sys::get_tick();
    /* USB初始化 */
    MX_USB_DEVICE_Init();

    /* 关闭两个24V  打开5V*/
    power_24v_right.WriteDown();
    power_24v_left.WriteDown();
    power_5v.WriteUp();

    SuperIWDG::instance().update();

    /* W25Q64初始化 */
    // w25q64.init();
    /* CAN初始化 */
    canPlus1.filter_config(1);
    canPlus2.filter_config(1);
    canPlus3.filter_config(2);
    canPlus1.start();
    canPlus2.start();
    canPlus3.start();
    SuperIWDG::instance().update();
    /* 遥控器和键鼠初始化 */

    /* 蜂鸣器初始化 */
    buzzer.Start();
    ada = Sys::get_tick() - ada;

    vTaskDelete(NULL);
}
