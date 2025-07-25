//
// Created by Administrator on 25-5-19.
//
#include "Chassis/Chassis.hpp"
#include "Interact/Interact.hpp"

void chassis_w_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.ySlope.step_set(chassis_dep::normal_speed_step);
            chassis.key.w = 0.4;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.w = 0;
            if (is_equal<0.01f>(chassis.key.w,0) && is_equal<0.01f>(chassis.key.s,0)) chassis.move.ySlope.step_set(chassis_dep::stop_speed_step);
            break;
        default: break;
    }
}

void chassis_a_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.xSlope.step_set(chassis_dep::normal_speed_step);
            chassis.key.a = -0.4;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.a = 0;
            if (is_equal<0.01f>(chassis.key.a,0) && is_equal<0.01f>(chassis.key.d,0)) chassis.move.xSlope.step_set(chassis_dep::stop_speed_step);
            break;
        default: break;
    }
}

void chassis_s_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.ySlope.step_set(chassis_dep::normal_speed_step);
            chassis.key.s = -0.4;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.s = 0;
            if (is_equal<0.01f>(chassis.key.w,0) && is_equal<0.01f>(chassis.key.s,0)) chassis.move.ySlope.step_set(chassis_dep::stop_speed_step);
            break;
        default: break;
    }
}

void chassis_d_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.xSlope.step_set(chassis_dep::normal_speed_step);
            chassis.key.d = 0.4;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp:
            chassis.key.d = 0;
            if (is_equal<0.01f>(chassis.key.a,0) && is_equal<0.01f>(chassis.key.d,0)) chassis.move.xSlope.step_set(chassis_dep::stop_speed_step);
            break;
        default: break;
    }
}

void chassis_shift_w_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.ySlope.step_set(chassis_dep::shift_speed_step);
            chassis.key.w = 0.1;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.w = 0; break;
        default: break;
    }
}

void chassis_shift_a_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.xSlope.step_set(chassis_dep::shift_speed_step);
            chassis.key.a = -0.1;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.a = 0; break;
        default: break;
    }
}

void chassis_shift_s_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.ySlope.step_set(chassis_dep::shift_speed_step);
            chassis.key.s = -0.1;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.s = 0; break;
        default: break;
    }
}

void chassis_shift_d_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.xSlope.step_set(chassis_dep::shift_speed_step);
            chassis.key.d = 0.1;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.d = 0; break;
        default: break;
    }
}

void chassis_ctrl_w_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.ySlope.step_set(chassis_dep::fast_speed_step);
            chassis.key.w = 0.9;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.w = 0; break;
        default: break;
    }
}

void chassis_ctrl_a_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.xSlope.step_set(chassis_dep::fast_speed_step);
            chassis.key.a = -0.9;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.a = 0; break;
        default: break;
    }
}

void chassis_ctrl_s_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.ySlope.step_set(chassis_dep::fast_speed_step);
            chassis.key.s = -0.9;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.s = 0; break;
        default: break;
    }
}

void chassis_ctrl_d_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing:
            chassis.move.xSlope.step_set(chassis_dep::fast_speed_step);
            chassis.key.d = 0.9;
            break;
        case KeyEvent_None:
        case KeyEvent_OnUp: chassis.key.d = 0; break;
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

void robo_arm_ctrl_e_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick: interact.sub_board.toggle_chassis_valve(); break;
        default: break;
    }
}
