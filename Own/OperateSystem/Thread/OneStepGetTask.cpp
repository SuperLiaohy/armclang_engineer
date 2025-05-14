//
// Created by Administrator on 25-5-2.
//
#include "Interact/Interact.hpp"
#include "Motor/M2006Diff.hpp"
#include "Motor/Param/M2006Diff.hpp"
#include "OneStepGet/OneStepGet.hpp"
void OneStepGetTask() {
    // float left_y  = 0;
    // float left_x  = 0;
    // float right_y = 0;
    // float right_x = 0;
    //
    // uint16_t left_block_count = 0;

    // float left_out_x  = 0;
    // float left_out_y  = 0;
    // float right_out_x = 0;
    // float right_out_y = 0;
    //
    // one_step_gets.left.X.pos.target_set(3000);
    // one_step_gets.left.Y.pos.target_set(left_x);
    //
    while (1) {
        if (OSG::mode == OneStepGetControl::MANUAL) {
            switch (one_step_gets.left.X.status) {
                case OneStepGetXStatus::NONE:
                    one_step_gets.left.X.Motor.set_position(one_step_gets.left.X.pos.get());
                    break;
                case OneStepGetXStatus::FRONT:
                    if (one_step_gets.left.X.Motor.is_block(-2500, 2500)) {
                        if (++one_step_gets.left.X.block_count > 1) { one_step_gets.left.X.is_block = true; }
                    } else {
                        one_step_gets.left.X.block_count = 0;
                        one_step_gets.left.X.is_block    = false;
                    }
                    one_step_gets.left.X.move_it();
                    break;
                case OneStepGetXStatus::BACK:
                    if (one_step_gets.left.X.Motor.is_block(-3500, 5500)) {
                        if (++one_step_gets.left.X.block_count > 1) { one_step_gets.left.X.is_block = true; }
                    } else {
                        one_step_gets.left.X.block_count = 0;
                        one_step_gets.left.X.is_block    = false;
                    }
                    one_step_gets.left.X.move_it();
                    break;
            }
            switch (one_step_gets.left.Y.status) {
                case OneStepGetYStatus::NONE:
                    one_step_gets.left.Y.Motor.set_position(one_step_gets.left.Y.pos.get());
                    break;
                case OneStepGetYStatus::UP:
                    if (one_step_gets.left.Y.Motor.is_block(-3500, 3500)) {
                        if (++one_step_gets.left.Y.block_count > 1) { one_step_gets.left.Y.is_block = true; }
                    } else {
                        one_step_gets.left.Y.block_count = 0;
                        one_step_gets.left.Y.is_block    = false;
                    }
                    one_step_gets.left.Y.move_it();
                    break;
                case OneStepGetYStatus::DOWN:
                    if (one_step_gets.left.Y.Motor.is_block(-3500, 3500)) {
                        if (++one_step_gets.left.Y.block_count > 1) { one_step_gets.left.Y.is_block = true; }
                    } else {
                        one_step_gets.left.Y.block_count = 0;
                        one_step_gets.left.Y.is_block    = false;
                    }
                    one_step_gets.left.Y.move_it();
                    break;
            }
            switch (one_step_gets.right.X.status) {
                case OneStepGetXStatus::NONE:
                    one_step_gets.right.X.Motor.set_position(one_step_gets.right.X.pos.get());
                    break;
                case OneStepGetXStatus::FRONT:
                    if (one_step_gets.right.X.Motor.is_block(-2500, 2500)) {
                        if (++one_step_gets.right.X.block_count > 1) { one_step_gets.right.X.is_block = true; }
                    } else {
                        one_step_gets.right.X.block_count = 0;
                        one_step_gets.right.X.is_block    = false;
                    }
                    one_step_gets.right.X.move_it();
                    break;
                case OneStepGetXStatus::BACK:
                    if (one_step_gets.right.X.Motor.is_block(-3500, 3500)) {
                        if (++one_step_gets.right.X.block_count > 1) { one_step_gets.right.X.is_block = true; }
                    } else {
                        one_step_gets.right.X.block_count = 0;
                        one_step_gets.right.X.is_block    = false;
                    }
                    one_step_gets.right.X.move_it();
                    break;
            }
            switch (one_step_gets.right.Y.status) {
                case OneStepGetYStatus::NONE:
                    one_step_gets.right.Y.Motor.set_position(one_step_gets.right.Y.pos.get());
                    break;
                case OneStepGetYStatus::UP:
                    if (one_step_gets.right.Y.Motor.is_block(-3500, 3500)) {
                        if (++one_step_gets.right.Y.block_count > 1) { one_step_gets.right.Y.is_block = true; }
                    } else {
                        one_step_gets.right.Y.block_count = 0;
                        one_step_gets.right.Y.is_block    = false;
                    }
                    one_step_gets.right.Y.move_it();
                    break;
                case OneStepGetYStatus::DOWN:
                    if (one_step_gets.right.Y.Motor.is_block(-3500, 3500)) {
                        if (++one_step_gets.right.Y.block_count > 1) { one_step_gets.right.Y.is_block = true; }
                    } else {
                        one_step_gets.right.Y.block_count = 0;
                        one_step_gets.right.Y.is_block    = false;
                    }
                    one_step_gets.right.Y.move_it();
                    break;
            }
        } else if (OSG::mode == OneStepGetControl::AUTO) {
            // if (OSG::auto_mode == OneStepGetAUTO::GOT_X) {
            //     one_step_gets.left.Y.move_up();
            //     one_step_gets.right.Y.move_up();
            //     bool left_get =
            //         isInRange(one_step_gets.left.Y.pos.target, one_step_gets.left.Y.Motor.total_position() - 15,
            //                   one_step_gets.left.Y.Motor.total_position() + 15);
            //     bool right_get =
            //         isInRange(one_step_gets.right.Y.pos.target, one_step_gets.right.Y.Motor.total_position() - 15,
            //                   one_step_gets.right.Y.Motor.total_position() + 15);
            //     if (left_get && right_get) {
            //         // switch (one_step_get_right.x) {
            //         //     case OneStepGetXStatus::NONE: right_out_x = one_step_get_right.move_front(0, false); break;
            //         //     case OneStepGetXStatus::FRONT: right_out_x = one_step_get_right.move_front(-4000, false); break;
            //         //     case OneStepGetXStatus::BACK: right_out_x = one_step_get_right.move_back(4000); break;
            //         // }
            //         // switch (one_step_get_left.x) {
            //         //     case OneStepGetXStatus::NONE: left_out_x = one_step_get_left.move_front(0, false); break;
            //         //     case OneStepGetXStatus::FRONT: left_out_x = one_step_get_left.move_front(4000, false); break;
            //         //     case OneStepGetXStatus::BACK: left_out_x = one_step_get_left.move_back(-4000); break;
            //         // }
            //     }
            // }
            switch (one_step_gets.left.X.status) {
                case OneStepGetXStatus::FRONT:
                    if (one_step_gets.left.X.Motor.is_block(-4500, 4500)) {
                        if (++one_step_gets.left.X.block_count > 1) { one_step_gets.left.X.is_block = true; }
                    } else {
                        one_step_gets.left.X.block_count = 0;
                        one_step_gets.left.X.is_block    = false;
                    }
                    break;
                case OneStepGetXStatus::BACK:
                    if (one_step_gets.left.X.Motor.is_block(-4500, 4500)) {
                        if (++one_step_gets.left.X.block_count > 1) { one_step_gets.left.X.is_block = true; }
                    } else {
                        one_step_gets.left.X.block_count = 0;
                        one_step_gets.left.X.is_block    = false;
                    }
                    break;
                default:break;
            }

            switch (one_step_gets.left.Y.status) {
                case OneStepGetYStatus::UP:
                    if (one_step_gets.left.Y.Motor.is_block(-5500, 5500)) {
                        if (++one_step_gets.left.Y.block_count > 1) { one_step_gets.left.Y.is_block = true; }
                    } else {
                        one_step_gets.left.Y.block_count = 0;
                        one_step_gets.left.Y.is_block    = false;
                    }
                    break;
                case OneStepGetYStatus::DOWN:
                    if (one_step_gets.left.Y.Motor.is_block(-5500, 5500)) {
                        if (++one_step_gets.left.Y.block_count > 1) { one_step_gets.left.Y.is_block = true; }
                    } else {
                        one_step_gets.left.Y.block_count = 0;
                        one_step_gets.left.Y.is_block    = false;
                    }
                    break;
                default:;
            }
            switch (one_step_gets.right.X.status) {
                case OneStepGetXStatus::FRONT:
                    if (one_step_gets.right.X.Motor.is_block(-2500, 2500)) {
                        if (++one_step_gets.right.X.block_count > 1) { one_step_gets.right.X.is_block = true; }
                    } else {
                        one_step_gets.right.X.block_count = 0;
                        one_step_gets.right.X.is_block    = false;
                    }
                    break;
                case OneStepGetXStatus::BACK:
                    if (one_step_gets.right.X.Motor.is_block(-3500, 3500)) {
                        if (++one_step_gets.right.X.block_count > 1) { one_step_gets.right.X.is_block = true; }
                    } else {
                        one_step_gets.right.X.block_count = 0;
                        one_step_gets.right.X.is_block    = false;
                    }
                    break;
                default:break;
            }

            switch (one_step_gets.right.Y.status) {
                case OneStepGetYStatus::UP:
                    if (one_step_gets.right.Y.Motor.is_block(-3500, 3500)) {
                        if (++one_step_gets.right.Y.block_count > 1) { one_step_gets.right.Y.is_block = true; }
                    } else {
                        one_step_gets.right.Y.block_count = 0;
                        one_step_gets.right.Y.is_block    = false;
                    }
                    break;
                case OneStepGetYStatus::DOWN:
                    if (one_step_gets.right.Y.Motor.is_block(-3500, 3500)) {
                        if (++one_step_gets.right.Y.block_count > 1) { one_step_gets.right.Y.is_block = true; }
                    } else {
                        one_step_gets.right.Y.block_count = 0;
                        one_step_gets.right.Y.is_block    = false;
                    }
                    break;
                default:break;
            }

            one_step_gets.left.X.move_it();
            one_step_gets.left.Y.move_it();

            one_step_gets.right.X.move_it();
            one_step_gets.right.Y.move_it();

        }
        canPlus3.transmit(0x200, one_step_gets.right.Y.Motor.speed_output(), one_step_gets.left.Y.Motor.speed_output(),
            one_step_gets.right.X.Motor.speed_output(), one_step_gets.left.X.Motor.speed_output());
        osDelay(1);
    }
}