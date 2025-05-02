//
// Created by Administrator on 25-2-16.
//
#include "Interact/Interact.hpp"
#include "OneStepGet/OneStepGet.hpp"

void one_step_get_z_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: one_step_get_left.x = OneStepGetXStatus::FRONT; break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            one_step_get_left.x = OneStepGetXStatus::NONE;
            break;
            break;
        default: break;
    }
}

void one_step_get_shift_z_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: one_step_get_left.x = OneStepGetXStatus::BACK; break;
        case KeyEvent_None:
        case KeyEvent_OnUp: one_step_get_left.x = OneStepGetXStatus::NONE; break;
        default: break;
    }
}

void one_step_get_x_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            one_step_get_left.y           = OneStepGetYStatus::UP;
            one_step_get_right.y_is_block = false;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: break;
        default: break;
    }
}

void one_step_get_shift_x_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            one_step_get_left.y           = OneStepGetYStatus::DOWN;
            one_step_get_right.y_is_block = false;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: break;
        default: break;
    }
}

void one_step_get_c_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: one_step_get_right.x = OneStepGetXStatus::FRONT; break;
        case KeyEvent_None:
        case KeyEvent_OnUp: one_step_get_right.x = OneStepGetXStatus::NONE; break;
        default: break;
    }
}

void one_step_get_shift_c_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: one_step_get_right.x = OneStepGetXStatus::BACK; break;
        case KeyEvent_None:
        case KeyEvent_OnUp: one_step_get_right.x = OneStepGetXStatus::NONE; break;
        default: break;
    }
}

void one_step_get_v_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            one_step_get_right.y          = OneStepGetYStatus::UP;
            one_step_get_right.y_is_block = false;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: break;
        default: break;
    }
}

void one_step_get_shift_v_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            one_step_get_right.y          = OneStepGetYStatus::DOWN;
            one_step_get_right.y_is_block = false;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: break;
        default: break;
    }
}

uint32_t cnt_right;
void air_left_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            interact.sub_board.set_valve1(1);
            // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            interact.sub_board.set_valve1(0);
            // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            break;
        default: break;
    }
}

void air_right_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            static uint8_t pump = 0;
            pump                = 1 - pump;
            interact.sub_board.set_pump(pump);
            ++cnt_right;
            break;
        default: break;
    }
}

void robo_arm_shift_q_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            if (interact.robo_arm.mode != interact_dep::robo_mode::CUSTOM) {
                interact.robo_arm.mode = interact_dep::robo_mode::CUSTOM;
            }
            break;
        default: break;
    }
}

void robo_arm_ctrl_q_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
        case KeyEvent_OnClick:
            if (interact.robo_arm.mode != interact_dep::robo_mode::VISION) {
                interact.robo_arm.mode = interact_dep::robo_mode::VISION;
            }
            break;
        default: break;
    }
}

void robo_arm_shift_f_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick: interact.image_trans.toggle_map_back(); break;
        default: break;
    }
}

void robo_arm_shift_r_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: interact.robo_arm.mode = interact_dep::robo_mode::RESET; break;
        default: break;
    }
}

void chassis_w_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: chassis.key.w = 1; break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.w = 0; break;
        default: break;
    }
}

void chassis_a_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: chassis.key.a = -1; break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.a = 0; break;
        default: break;
    }
}

void chassis_s_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: chassis.key.s = -1; break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.s = 0; break;
        default: break;
    }
}

void chassis_d_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: chassis.key.d = 1; break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.d = 0; break;
        default: break;
    }
}

void chassis_q_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            if (interact.chassis.mode != interact_dep::chassis_mode::CLIMB) {
                interact.chassis.mode = interact_dep::chassis_mode::CLIMB;
            } else {
                interact.chassis.mode = interact_dep::chassis_mode::NONE;
            }
            break;
        default: break;
    }
}
