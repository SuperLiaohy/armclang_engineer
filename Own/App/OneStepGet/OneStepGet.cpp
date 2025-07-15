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




bool OSG::group::XGet::move_it() {
    switch (status) {
        case OneStepGetXStatus::NONE: break;
        case OneStepGetXStatus::BACK: {
            auto& target_pos = pos.update();
            if (is_block) {
                this->status           = OneStepGetXStatus::NONE;
                Motor.total_position() = 0;
                pos.target_set(Motor.total_position());
                target_pos = Motor.total_position();
                Motor.set_position(target_pos);
                return false;
            }
        } break;
        case OneStepGetXStatus::FRONT: {
            auto& target_pos = pos.update();
            if (is_block) {
                this->status = OneStepGetXStatus::NONE;
                pos.target_set(Motor.total_position());
                target_pos = Motor.total_position();
                Motor.set_position(target_pos);
                return false;
            }
        } break;
    }
    Motor.set_position(pos.get());
    return true;
}

bool OSG::group::YGet::move_it() {
    switch (status) {
        case OneStepGetYStatus::NONE: break;
        case OneStepGetYStatus::DOWN: {
            auto& target_pos = pos.update();
            if (is_block) {
                this->status           = OneStepGetYStatus::NONE;
                Motor.total_position() = 0;
                pos.target_set(Motor.total_position());
                target_pos = Motor.total_position();
                Motor.set_position(target_pos);
                return false;
            }
        } break;
        case OneStepGetYStatus::UP: {
            auto& target_pos = pos.update();
            if (is_block) {
                this->status = OneStepGetYStatus::NONE;
                pos.target_set(Motor.total_position());
                target_pos = Motor.total_position();
                Motor.set_position(target_pos);
                return false;
            }
        } break;
    }
    Motor.set_position(pos.get());
    return true;
}
