//
// Created by Administrator on 25-5-1.
//

#include "OneStepGet.hpp"

float OneStepGet::move_front(const float target_speed, const bool is_get) {
    if (is_get || XMotor.is_block(4000)) return 0;
    return XMotor.set_speed(target_speed);
}
float OneStepGet::move_back(float target_speed) {
    if (XMotor.is_block(4000)) return 0;
    return XMotor.set_speed(target_speed);
}
float OneStepGet::move_upward(float& target_pos) {
    if (YMotor.is_block(10000) ) {
        y_is_block = true;
        this->y = OneStepGetYStatus::NONE;
        target_pos = YMotor.total_position();
        return YMotor.set_position(target_pos);
    }
    return YMotor.set_position(target_pos);
}
float OneStepGet::move_down(float& target_pos) {
    if (YMotor.is_block(10000)) {
        y_is_block = true;
        this->y = OneStepGetYStatus::NONE;
        target_pos = YMotor.total_position();
        return YMotor.set_position(target_pos);
    }
    return YMotor.set_position(target_pos);
}
