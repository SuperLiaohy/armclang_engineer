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

void robo_arm_e_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
        case KeyEvent_OnClick:
            test_actions.now = 0;
            interact.actions = &test_actions;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        default:
            break;
    }
}

void robo_arm_shift_e_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            interact.robo_arm.mode = interact_dep::robo_mode::NONE;
            break;
        default:
            break;
    }
}

void robo_arm_shift_q_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            if (interact.robo_arm.mode != interact_dep::robo_mode::CUSTOM) {
                interact.path = interact_dep::path::IMAGE_TRANSMIT;
                interact.robo_arm.mode = interact_dep::robo_mode::CUSTOM;
            } else {
                interact.path = interact_dep::path::REMOTE_CTRL;
                interact.chassis.mode = interact_dep::chassis_mode::NONE;
            }
            break;
        default:
            break;

    }
}

void robo_arm_shift_c_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            interact.image_trans.toggle_map_back();
            break;
        default:
            break;
    }
}

