//
// Created by lhy on 2024/9/15.
//

#include "RoundQueue.h"


float RoundQueue::pop() {
    if (head == tail) {
        return 0;
    }
    const auto value = buffer[tail];
    tail = static_cast<int8_t>(((tail + 1) % size));
    return value;
}

float RoundQueue::push(float value) {
    buffer[head] = value;
    head = static_cast<int8_t>(((head + 1) % size));
    if (head == tail) {
        value -= buffer[tail];
        tail = static_cast<int8_t>(((tail + 1) % size));
    }
    return value;
}
