//
// Created by Administrator on 25-2-16.
//
#include "Interact/interact.hpp"
#include "Pump/Pump.hpp"
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
            stretch.now = 0;
            interact.actions = &stretch;
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
            interact.path = interact_dep::path::IMAGE_TRANSMIT;
            if (interact.robo_arm.mode != interact_dep::robo_mode::CUSTOM) {
                interact.robo_arm.mode = interact_dep::robo_mode::CUSTOM;
            } else {
                interact.path = interact_dep::path::REMOTE_CTRL;
                interact.robo_arm.mode = interact_dep::robo_mode::NONE;
            }
            break;
        default:
            break;

    }
}

void robo_arm_ctrl_q_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
        case KeyEvent_OnClick:
            interact.path = interact_dep::path::PC;
            if (interact.robo_arm.mode != interact_dep::robo_mode::VISION) {
                interact.robo_arm.mode = interact_dep::robo_mode::VISION;
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

void robo_arm_r_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
        case KeyEvent_OnClick:
            turn_up.now = 0;
            interact.actions = &turn_up;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        default:
            break;
    }
}

void robo_arm_shift_r_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            interact.robo_arm.mode = interact_dep::robo_mode::RESET;
            break;
        default:
            break;
    }
}

void chassis_w_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.key.w = 1;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.w = 0;
            break;
        default:
            break;
    }
}

void chassis_a_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.key.a = -1;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.a = 0;
            break;
        default:
            break;
    }
}

void chassis_s_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.key.s = -1;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.s = 0;
            break;
        default:
            break;
    }
}

void chassis_d_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.key.d = 1;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.d = 0;
            break;
        default:
            break;
    }
}

void chassis_q_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            if (interact.chassis.mode != interact_dep::chassis_mode::ALL) {
                interact.chassis.mode = interact_dep::chassis_mode::ALL;
            } else {
                interact.chassis.mode = interact_dep::chassis_mode::NONE;
            }
            break;
        default:
            break;

    }
}
