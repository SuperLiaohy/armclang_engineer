//
// Created by Administrator on 25-5-2.
//
#include "Interact/Interact.hpp"
#include "Motor/M2006Diff.hpp"
#include "Motor/Param/M2006Diff.hpp"
#include "OneStepGet/OneStepGet.hpp"
void OneStepGetTask() {
    float left_y  = 0;
    float right_y = 0;

    uint16_t left_block_count = 0;

    float left_out_x  = 0;
    float left_out_y  = 0;
    float right_out_x = 0;
    float right_out_y = 0;
    while (1) {
        switch (one_step_get_left.x) {
            case OneStepGetXStatus::NONE: left_out_x = one_step_get_left.move_front(0, false); break;
            case OneStepGetXStatus::FRONT: left_out_x = one_step_get_left.move_front(4000, false); break;
            case OneStepGetXStatus::BACK: left_out_x = one_step_get_left.move_back(-4000); break;
        }
        bool tick = false;
        switch (one_step_get_left.y) {
            case OneStepGetYStatus::NONE: right_out_y = one_step_get_right.YMotor.set_position(right_y); break;
            case OneStepGetYStatus::UP:
                left_y += 1;

                if (one_step_get_left.YMotor.is_block(15000)) {
                    if (++left_block_count > 0) { tick = true; }
                } else {
                    left_block_count = 0;
                }
                left_out_y = one_step_get_left.move_upward(left_y, tick);
                break;
            case OneStepGetYStatus::DOWN:
                left_y -= 1;
                if (one_step_get_left.YMotor.is_block(15000)) {
                    if (++left_block_count > 0) { tick = true; }
                } else {
                    left_block_count = 0;
                }
                left_out_y = one_step_get_left.move_down(left_y, false, tick);
                break;
        }
        switch (one_step_get_right.x) {
            case OneStepGetXStatus::NONE: right_out_x = one_step_get_right.move_front(0, false); break;
            case OneStepGetXStatus::FRONT: right_out_x = one_step_get_right.move_front(-4000, false); break;
            case OneStepGetXStatus::BACK: right_out_x = one_step_get_right.move_back(4000); break;
        }
        switch (one_step_get_right.y) {
            case OneStepGetYStatus::NONE: right_out_y = one_step_get_right.YMotor.set_position(right_y); break;
            case OneStepGetYStatus::UP:
                --right_y;
                right_out_y = one_step_get_right.move_upward(right_y, one_step_get_right.YMotor.is_block(14000));
                break;
            case OneStepGetYStatus::DOWN:
                ++right_y;
                right_out_y = one_step_get_right.move_down(right_y, false, one_step_get_right.YMotor.is_block(14000));
                break;
        }
        canPlus3.transmit(0x200, right_out_y, left_out_y, right_out_x, left_out_x);
        osDelay(1);
    }
}