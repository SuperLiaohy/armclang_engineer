//
// Created by Administrator on 25-5-1.
//

#include "OneStepGet.hpp"

float OneStepGet::move_front(float& target_pos, const bool is_get, const bool is_block) {
    if (is_get || is_block) {
        x_is_block = true;
        this->x = OneStepGetXStatus::NONE;
        target_pos = XMotor.total_position();
        return XMotor.set_position(target_pos);
    }
    return XMotor.set_position(target_pos);
}
float OneStepGet::move_back(float& target_pos, const bool is_block) {
    if (is_block) {
        this->x = OneStepGetXStatus::NONE;
        target_pos = XMotor.total_position();
        return XMotor.set_position(target_pos);
    }
    return XMotor.set_position(target_pos);
}
float OneStepGet::move_upward(float& target_pos, bool is_block) {
    if (is_block) {
        y_is_block = true;
        this->y    = OneStepGetYStatus::NONE;
        target_pos = YMotor.total_position();
        return YMotor.set_position(target_pos);
    }
    return YMotor.set_position(target_pos);
}
float OneStepGet::move_down(float& target_pos, bool is_get, bool is_block) {
    if (is_block || is_get) {
        y_is_block = true;
        this->y    = OneStepGetYStatus::NONE;
        target_pos = YMotor.total_position();
        return YMotor.set_position(target_pos);
    }
    return YMotor.set_position(target_pos);
}
