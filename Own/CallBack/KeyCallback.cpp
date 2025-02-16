//
// Created by Administrator on 25-2-16.
//
#include "Interact/interact.h"
#include "Pump/Pump.h"
uint32_t cnt_right;
void air_left_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

            break;
        default:
            break;
    }
}


void air_right_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            pump.toggle();
						++cnt_right;
            break;
        default:
            break;
    }
}
