//
// Created by lhy on 2024/9/15.
//

#include "Slope.h"

float Slope::update() {
    if (target - value < dead_zone && target - value > -dead_zone) { return value; }

    if (target > value) {
        value += step;
        if (value > target) { value = target; }
    } else if (target < value) {
        value -= step;
        if (value < target) { value = target; }
    }
    return value;
}
