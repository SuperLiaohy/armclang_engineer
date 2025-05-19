//
// Created by Administrator on 25-5-19.
//

#include "Interact/Interact.hpp"
#include "OneStepGet/OneStepGet.hpp"

extern interact_dep::ActionsGroup get_gold_group;
extern interact_dep::ActionsGroup arm_get_gold_group;

void action_z_callback(KeyEventType event) {
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
                get_gold_group.reset();
                interact.actions_group = &get_gold_group;
                interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS_GROUP;

                // arm_get_gold_group.reset();
                // interact.actions_group = &arm_get_gold_group;
                // interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS_GROUP;

                break;
            default: break;
        }
    }
}

void action_shift_z_callback(KeyEventType event) {
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

extern interact_dep::ActionsGroup get_silver_group;

void action_x_callback(KeyEventType event) {
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
        switch (event) {
            case KeyEvent_OnClick:
                get_silver_group.reset();
                interact.actions_group = &get_silver_group;
                interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS_GROUP;

                break;
            default: break;
        }
    }
}
extern interact_dep::ActionsGroup get_silver_from_left_group;
void action_shift_x_callback(KeyEventType event) {
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
        switch (event) {
            case KeyEvent_OnClick:
                get_silver_from_left_group.reset();
                interact.actions_group = &get_silver_from_left_group;
                interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS_GROUP;

                break;
            default: break;
        }
    }
}
extern interact_dep::ActionsGroup get_second_silver_group;

extern interact_dep::Actions reset2;
void action_c_callback(KeyEventType event) {
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
        switch (event) {
            case KeyEvent_OnClick:
                get_second_silver_group.reset();
                interact.actions_group = &get_second_silver_group;
                interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS_GROUP;
                break;
            default: break;
        }
    }
}
extern interact_dep::ActionsGroup put_down_group;

void action_shift_c_callback(KeyEventType event) {
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
        switch (event) {
            case KeyEvent_OnClick:
                put_down_group.reset();
                interact.actions_group = &put_down_group;
                interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS_GROUP;
                break;
            default: break;
        }
    }
}

void action_v_callback(KeyEventType event) {
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
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                reset2.init            = false;
                interact.actions       = &reset2;
                interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
                break;
            default: break;
        }
    }
}

extern interact_dep::ActionsGroup ready_silver2_group;
void action_shift_v_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnDown:
            case KeyEvent_OnLongPress:
            case KeyEvent_OnPressing:
                one_step_gets.right.X.pos.target_set(2300);
                one_step_gets.right.X.status = OneStepGetXStatus::BACK;
                break;
            case KeyEvent_None:
            case KeyEvent_OnUp: one_step_gets.right.X.status = OneStepGetXStatus::NONE; break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
        switch (event) {
            case KeyEvent_OnClick:
                ready_silver2_group.reset();
                interact.actions_group = &ready_silver2_group;
                interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS_GROUP;
                break;
            default: break;
        }
    }
}

void action_ctrl_z_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnClick:
                interact.sub_board.toggle_lf_valve();
                break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}
void action_ctrl_x_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnClick:
                interact.sub_board.toggle_lb_valve();
                break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}
void action_ctrl_c_callback(KeyEventType event) {
    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnClick:
                interact.sub_board.toggle_rb_valve();
                break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
    }
}
extern interact_dep::ActionsGroup get_silver2_group;
void action_ctrl_v_callback(KeyEventType event) {

    if (OSG::mode == OneStepGetControl::MANUAL) {
        switch (event) {
            case KeyEvent_OnClick:
                interact.sub_board.toggle_rf_valve();
                break;
            default: break;
        }
    } else if (OSG::mode == OneStepGetControl::AUTO) {
        switch (event) {
            case KeyEvent_OnClick:
                get_silver2_group.reset();
                interact.actions_group = &get_silver2_group;
                interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS_GROUP;
                break;
            default: break;
        }
    }
}

void action_b_callback(KeyEventType event) {
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

void air_left_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            interact.sub_board.toggle_main_valve();
            break;
        default: break;
    }
}

void air_right_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            interact.sub_board.toggle_pump();
            break;
        default: break;
    }
}

void action_ctrl_r_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            one_step_gets.reset();
            break;
        default: break;
    }
}

extern interact_dep::Actions get_right_y;
extern interact_dep::Actions get_silver_mine;
extern interact_dep::Actions get_silver_mine_z;
void action_shift_f_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            get_silver_mine.init   = false;
            interact.actions       = &get_silver_mine;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        default: break;
    }
}
void action_ctrl_f_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            get_silver_mine_z.init = false;
            interact.actions       = &get_silver_mine_z;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        default: break;
    }
}

extern interact_dep::Actions exchange_left;
void action_e_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            exchange_left.init     = false;
            interact.actions       = &exchange_left;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        default: break;
    }
}
extern interact_dep::Actions exchange_right;
void action_f_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            get_silver_mine_z.init = false;
            interact.actions       = &exchange_right;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
            break;
        default: break;
    }
}
extern interact_dep::Actions reset1;
void action_shift_r_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            reset1.init            = false;
            interact.actions       = &reset1;
            interact.robo_arm.mode = interact_dep::robo_mode::ACTIONS;
        default: break;
    }
}

