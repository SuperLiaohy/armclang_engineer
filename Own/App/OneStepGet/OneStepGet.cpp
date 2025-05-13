//
// Created by Administrator on 25-5-1.
//

#include "OneStepGet.hpp"

bool OSG::group::XGet::move_back() {
    switch (status) {
        case OneStepGetXStatus::NONE: break;
        default:
            auto& target_pos = pos.update();
            if (is_block) {
                this->status           = OneStepGetXStatus::NONE;
                Motor.total_position() = 0;
                pos.target_set(Motor.total_position());

                target_pos = Motor.total_position();
                Motor.set_position(target_pos);
                return false;
            }
            break;
    }
    Motor.set_position(pos.get());
    return true;
}

bool OSG::group::XGet::move_front() {
    switch (status) {
        case OneStepGetXStatus::NONE: break;
        default:
            auto& target_pos = pos.update();
            if (is_block) {
                this->status = OneStepGetXStatus::NONE;
                pos.target_set(Motor.total_position());
                target_pos = Motor.total_position();
                Motor.set_position(target_pos);
                return false;
            }
            break;
    }
    Motor.set_position(pos.get());
    return true;
}

bool OSG::group::YGet::move_up() {
    switch (status) {
        case OneStepGetYStatus::NONE: break;
        default:
            auto& target_pos = pos.update();
            if (is_block) {
                this->status = OneStepGetYStatus::NONE;
                pos.target_set(Motor.total_position());
                target_pos = Motor.total_position();
                Motor.set_position(target_pos);
                return false;
            }
            break;
    }
    Motor.set_position(pos.get());
    return true;
}

bool OSG::group::YGet::move_down() {
    switch (status) {
        case OneStepGetYStatus::NONE: break;
        default:
            auto& target_pos = pos.update();
            if (is_block) {
                this->status           = OneStepGetYStatus::NONE;
                Motor.total_position() = 0;
                pos.target_set(Motor.total_position());
                target_pos = Motor.total_position();
                Motor.set_position(target_pos);
                return false;
            }
            break;
    }
    Motor.set_position(pos.get());
    return true;
}
