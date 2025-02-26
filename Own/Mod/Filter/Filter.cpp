//
// Created by lhy on 2024/9/15.
//

#include "Filter.hpp"


float Filter::update(const float value) {
    sum += queue.push(value);
    return sum / static_cast<float>(queue.len());
}
