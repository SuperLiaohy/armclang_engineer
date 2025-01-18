//
// Created by lhy on 2024/9/15.
//

#include "Chassis/Chassis.h"
#include "RemoteControl/RemoteControl.h"

#if USING_CHASSIS == 1


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
            if (chassis.mode == chassis_dep::Follow) {
                chassis.mode = chassis_dep::Work;
            } else {
                chassis.mode = chassis_dep::Follow;
            }
            break;
        default:
            break;

    }
}


#endif
