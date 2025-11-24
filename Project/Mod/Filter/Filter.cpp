//
// Created by lhy on 2024/9/15.
//

#include "Filter.hpp"

template<uint32_t n>
float Filter<n>::update(const float value) {
    if (queue.len() == n) {
        float front;
        queue.pop(front);
        sum -= front;
    }
    queue.push(value);
    sum += value;
    return sum  / static_cast<float>(queue.len());
}
