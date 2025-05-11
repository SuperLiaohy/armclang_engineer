//
// Created by Administrator on 25-2-16.
//
#include "Interact/Interact.hpp"
#include "OneStepGet/OneStepGet.hpp"

void one_step_get_z_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.left.X.pos.target_set(2000);
                one_step_gets.left.X.status = OneStepGetXStatus::FRONT;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.left.X.status = OneStepGetXStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
        switch (event) {
            case KeyEvent_OnClick:
                one_step_gets.right.X.status = OneStepGetXStatus::FRONT;
                one_step_gets.left.X.status  = OneStepGetXStatus::FRONT;
                OSG::auto_mode               = OneStepGetAUTO::GOT_X;
                break;
            default: break;
        }
    }
}

void one_step_get_shift_z_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.left.X.pos.target_set(-2000);
                one_step_gets.left.X.status = OneStepGetXStatus::BACK;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.left.X.status = OneStepGetXStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}

void one_step_get_x_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.left.Y.pos.target_set(1300);
                one_step_gets.left.Y.status = OneStepGetYStatus::UP;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.left.Y.status = OneStepGetYStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}

void one_step_get_shift_x_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.left.Y.pos.target_set(-1300);
                one_step_gets.left.Y.status = OneStepGetYStatus::DOWN;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.left.Y.status = OneStepGetYStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}

void one_step_get_c_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.right.Y.pos.target_set(-1450);
                one_step_gets.right.Y.status = OneStepGetYStatus::UP;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.right.Y.status = OneStepGetYStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}

void one_step_get_shift_c_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.right.Y.pos.target_set(1450);
                one_step_gets.right.Y.status = OneStepGetYStatus::DOWN;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.right.Y.status = OneStepGetYStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}

void one_step_get_v_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.right.X.pos.target_set(-2300);
                one_step_gets.right.X.status = OneStepGetXStatus::FRONT;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.right.X.status = OneStepGetXStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}

void one_step_get_shift_v_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.right.X.pos.target_set(-2300);
                one_step_gets.right.X.status = OneStepGetXStatus::FRONT;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.right.X.status = OneStepGetXStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}

void one_step_get_ctrl_z_callback(KeyEventType event) {
    static uint8_t open = 0;
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnClick:
                open = 1 - open;
                interact.sub_board.set_valve5(open);
                break;
                // case KeyEvent_OnDown:
                // case KeyEvent_OnLongPress:
                // case KeyEvent_OnPressing:
                //     interact.sub_board.set_valve5(1);
                //     break;
                // case KeyEvent_None:
                // case KeyEvent_OnUp:
                //     interact.sub_board.set_valve5(0);
                //     break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}
void one_step_get_ctrl_x_callback(KeyEventType event) {
    static uint8_t open = 0;
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnClick:
                open = 1 - open;
                interact.sub_board.set_valve4(open);
                break;
                // case KeyEvent_OnDown:
                // case KeyEvent_OnLongPress:
                // case KeyEvent_OnPressing:
                //     interact.sub_board.set_valve4(1);
                //     break;
                // case KeyEvent_None:
                // case KeyEvent_OnUp:
                //     interact.sub_board.set_valve4(0);
                //     break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}
void one_step_get_ctrl_c_callback(KeyEventType event) {
    static uint8_t open = 0;
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnClick:
                open = 1 - open;
                interact.sub_board.set_valve2(open);
                break;
                // case KeyEvent_OnDown:
                // case KeyEvent_OnLongPress:
                // case KeyEvent_OnPressing:
                //     interact.sub_board.set_valve2(1);
                //     break;
                // case KeyEvent_None:
                // case KeyEvent_OnUp:
                //     interact.sub_board.set_valve2(0);
                //     break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}
void one_step_get_ctrl_v_callback(KeyEventType event) {
    static uint8_t open = 0;
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnClick:
                open = 1 - open;
                interact.sub_board.set_valve1(open);
                break;
                // case KeyEvent_OnDown:
                // case KeyEvent_OnLongPress:
                // case KeyEvent_OnPressing:
                //     interact.sub_board.set_valve1(1);
                //     break;
                // case KeyEvent_None:
                // case KeyEvent_OnUp:
                //     interact.sub_board.set_valve1(0);
                //     break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}

void one_step_get_b_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            if (OSG::mode == OneStepGetControl::AUTO) {
                OSG::mode = OneStepGetControl::MANUAL;
            } else if (OSG::mode == OneStepGetControl::MANUAL) {
                OSG::mode = OneStepGetControl::AUTO;
            }
            break;
        default: break;
    }
}

uint32_t cnt_right;
void air_left_callback(KeyEventType event) {
    static uint8_t open = 0;
    switch (event) {
        case KeyEvent_OnClick:
            open = 1 - open;
            interact.sub_board.set_valve3(open);
            break;
        // case KeyEvent_OnDown:
        // case KeyEvent_OnLongPress:
        // case KeyEvent_OnPressing:
        //     interact.sub_board.set_valve3(1);
        //     // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        //     break;
        // case KeyEvent_None:
        // case KeyEvent_OnUp:
        //     interact.sub_board.set_valve3(0);
        //     // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        //     break;
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

void robo_arm_ctrl_g_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            interact.robo_arm.last_mode         = interact_dep::robo_mode::NONE;
            interact.robo_arm.mode              = interact_dep::robo_mode::NONE;
            interact.joint[5]                   = 0;
            interact.joint[4]                   = 0;
            roboArm.diff.left.total_position()  = 0;
            roboArm.diff.right.total_position() = 0;
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
extern interact_dep::Actions get_right_y;
extern interact_dep::Actions get_silver_mine;
extern interact_dep::Actions get_silver_mine_z;
void robo_arm_shift_f_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            interact.actions       = &get_silver_mine;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        // case KeyEvent_OnClick: interact.image_trans.toggle_map_back(); break;
        default: break;
    }
}
void robo_arm_ctrl_f_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            get_silver_mine_z.init = false;
            interact.actions       = &get_silver_mine_z;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        // case KeyEvent_OnClick: interact.image_trans.toggle_map_back(); break;
        default: break;
    }
}

extern interact_dep::Actions exchange_left;
void robo_arm_e_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            interact.actions       = &exchange_left;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
            // case KeyEvent_OnClick: interact.image_trans.toggle_map_back(); break;
        default: break;
    }
}
extern interact_dep::Actions exchange_right;
void robo_arm_f_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            interact.actions       = &exchange_right;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
            // case KeyEvent_OnClick: interact.image_trans.toggle_map_back(); break;
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

extern interact_dep::Actions anti_reset;

void robo_arm_ctrl_r_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            interact.actions       = &anti_reset;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        default: break;
    }
}

extern std::atomic<bool> ui_reset;
void robo_arm_r_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: ui_reset.store(true); break;
        default: ui_reset.store(false); break;
    }
}

void robo_arm_ctrl_e_callback(KeyEventType event) {
    static uint8_t open = 0;
    switch (event) {
        case KeyEvent_OnClick:
            open = 1 - open;
            interact.sub_board.set_valve6(open);
            break;
        default: break;
    }
}

void chassis_shift_e_callback(KeyEventType event) {
    static uint8_t open = 0;
    switch (event) {
        case KeyEvent_OnClick:
            open = 1 - open;
            if (open) {
                interact.polarity = interact_dep::chassis_polarity::ANTI;
            } else {
                interact.polarity = interact_dep::chassis_polarity::NONE;
            }
            break;
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

void chassis_shift_w_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.key.w = 0.1;
            chassis.move.ySlope.step_set(chassis_dep::shift_speed_step);
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.w = 0;
            chassis.move.ySlope.step_set(chassis_dep::normal_speed_step);
            break;
        default: break;
    }
}

void chassis_shift_a_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.key.a = -0.1;
            chassis.move.xSlope.step_set(chassis_dep::shift_speed_step);

            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.a = 0;
            chassis.move.xSlope.step_set(chassis_dep::normal_speed_step);

            break;

        default: break;
    }
}

void chassis_shift_s_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.key.s = -0.1;
            chassis.move.ySlope.step_set(chassis_dep::shift_speed_step);
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.s = 0;
            chassis.move.ySlope.step_set(chassis_dep::normal_speed_step);
            break;
        default: break;
    }
}

void chassis_shift_d_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.key.d = 0.1;
            chassis.move.xSlope.step_set(chassis_dep::shift_speed_step);
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.d = 0;
            chassis.move.xSlope.step_set(chassis_dep::normal_speed_step);
            break;
        default: break;
    }
}

void chassis_q_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            if (interact.chassis.mode != interact_dep::chassis_mode::NORMAL) {
                interact.chassis.mode = interact_dep::chassis_mode::NORMAL;
            } else {
                interact.chassis.mode = interact_dep::chassis_mode::NONE;
            }
            break;
        default: break;
    }
}
