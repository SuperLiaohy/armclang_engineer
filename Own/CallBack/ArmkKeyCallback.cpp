//
// Created by Administrator on 25-5-19.
//

#include "RoboArm/RoboArm.hpp"
#include "Interact/Interact.hpp"

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
void robo_arm_g_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:
            if (interact.robo_arm.mode != interact_dep::robo_mode::CUSTOM) {
                interact.robo_arm.mode = interact_dep::robo_mode::CUSTOM;
            } else {
                interact.robo_arm.mode = interact_dep::robo_mode::NONE;
            }
            break;
        default: break;
    }
}

void robo_arm_shift_q_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnClick:

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

extern std::atomic<bool> ui_reset;
void robo_arm_r_callback(KeyEventType event) {
    switch (event) {
        case KeyEvent_OnDown:
        case KeyEvent_OnLongPress:
        case KeyEvent_OnPressing: ui_reset.store(true); break;
        default: ui_reset.store(false); break;
    }
}
