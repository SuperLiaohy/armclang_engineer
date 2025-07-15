//
// Created by Administrator on 25-5-1.
//

#include "OneStepGet.hpp"

void Translation::move_handle() {
    Motor.set_position(axis.get());
};
void Translation::state_handle() {
    switch (s) {
        case state::P_BLOCK:
            if (pblock_count > -5)
                axis.decrease();
            else {
                pblock_count = 0;
                s = state::MOVE;
            }
            if (Motor.feedback.raw_data.current < pThresholdBlock) {
                --pblock_count;
            } else {
                ++pblock_count;
            }
            break;
        case state::N_BLOCK:
            if (pblock_count > -5)
                axis.increase();
            else {
                nblock_count = 0;
                s = state::MOVE;
            }
            if (Motor.feedback.raw_data.current > nThresholdBlock) {
                --nblock_count;
            } else {
                ++nblock_count;
            }
            break;
        case state::MOVE:
            axis.update();
            if (Motor.feedback.raw_data.current > pThresholdBlock) {
                if(++pblock_count > 20) {
                    s = state::P_BLOCK;
                };
            } else if (Motor.feedback.raw_data.current < nThresholdBlock) {
                if(++pblock_count > 20) {
                    s = state::N_BLOCK;
                };
            }
            break;
        case state::RESET:
            if (polarity) {
                if (Motor.feedback.raw_data.current < nThresholdBlock) {
                    if(++nblock_count > 10) {
                        Motor.total_position() = 0;
                        axis.get() = 0;
                        axis.target_set(0);
                        nblock_count = 10;
                        s = state::N_BLOCK;
                    };
                } else {
                    axis.target_set(-1000);
                }
            } else {
                if (Motor.feedback.raw_data.current > pThresholdBlock) {
                    if(++pblock_count > 10) {
                        Motor.total_position() = 0;
                        Motor.clear();
                        axis.get() = 0;
                        axis.target_set(0);
                        pblock_count = 10;
                        s = state::P_BLOCK;
                    } else {
                        axis.target_set(-1000);
                    };
                }
            }
            break;
    }
};
