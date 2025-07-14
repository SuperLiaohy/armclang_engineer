//
// Created by Administrator on 25-5-2.
//
#include "Interact/Interact.hpp"
#include "Motor/M2006Diff.hpp"
#include "Motor/Param/M2006Diff.hpp"
#include "OneStepGet/OneStepGet.hpp"
void OneStepGetTask() {

    while (1) {
        switch (one_step_gets.left.X.status) {
            case OneStepGetXStatus::FRONT:
                if (one_step_gets.left.X.Motor.is_block(-4000, 4000)) {
                    if (++one_step_gets.left.X.block_count > 1) { one_step_gets.left.X.is_block = true; }
                } else {
                    one_step_gets.left.X.block_count = 0;
                    one_step_gets.left.X.is_block    = false;
                }
                break;
            case OneStepGetXStatus::BACK:
                if (one_step_gets.left.X.Motor.is_block(-4000, 4000)) {
                    if (++one_step_gets.left.X.block_count > 1) { one_step_gets.left.X.is_block = true; }
                } else {
                    one_step_gets.left.X.block_count = 0;
                    one_step_gets.left.X.is_block    = false;
                }
                break;
            default: break;
        }

        switch (one_step_gets.left.Y.status) {
            case OneStepGetYStatus::UP:
                if (one_step_gets.left.Y.Motor.is_block(-6000, 6000)) {
                    if (++one_step_gets.left.Y.block_count > 1) { one_step_gets.left.Y.is_block = true; }
                } else {
                    one_step_gets.left.Y.block_count = 0;
                    one_step_gets.left.Y.is_block    = false;
                }
                break;
            case OneStepGetYStatus::DOWN:
                if (one_step_gets.left.Y.Motor.is_block(-6000, 6000)) {
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
                if (one_step_gets.right.X.Motor.is_block(-5500, 5500)) {
                    if (++one_step_gets.right.X.block_count > 5) { one_step_gets.right.X.is_block = true; }
                } else {
                    one_step_gets.right.X.block_count = 0;
                    one_step_gets.right.X.is_block    = false;
                }
                break;
            case OneStepGetXStatus::BACK:
                if (one_step_gets.right.X.Motor.is_block(-5500, 5500)) {
                    if (++one_step_gets.right.X.block_count > 5) { one_step_gets.right.X.is_block = true; }
                } else {
                    one_step_gets.right.X.block_count = 0;
                    one_step_gets.right.X.is_block    = false;
                }
                break;
            default: break;
        }

        switch (one_step_gets.right.Y.status) {
            case OneStepGetYStatus::UP:
                if (one_step_gets.right.Y.Motor.is_block(-7000, 7000)) {
                    if (++one_step_gets.right.Y.block_count > 1) { one_step_gets.right.Y.is_block = true; }
                } else {
                    one_step_gets.right.Y.block_count = 0;
                    one_step_gets.right.Y.is_block    = false;
                }
                break;
            case OneStepGetYStatus::DOWN:
                if (one_step_gets.right.Y.Motor.is_block(-7000, 7000)) {
                    if (++one_step_gets.right.Y.block_count > 1) { one_step_gets.right.Y.is_block = true; }
                } else {
                    one_step_gets.right.Y.block_count = 0;
                    one_step_gets.right.Y.is_block    = false;
                }
                break;
            default: break;
        }

        one_step_gets.left.X.move_it();
        one_step_gets.left.Y.move_it();

        one_step_gets.right.X.move_it();
        one_step_gets.right.Y.move_it();

        canPlus3.transmit(0x200, one_step_gets.right.Y.Motor.speed_output(), one_step_gets.left.Y.Motor.speed_output(),
            one_step_gets.right.X.Motor.speed_output(), one_step_gets.left.X.Motor.speed_output());
        osDelay(1);
    }
}