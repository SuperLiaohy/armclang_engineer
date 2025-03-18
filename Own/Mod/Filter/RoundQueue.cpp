//
// Created by lhy on 2024/9/15.
//

#include "RoundQueue.hpp"

template<typename T, uint32_t n>
bool RoundQueue<T, n>::pop(T& value) {
    if (length > 0) {
        value = buffer[head];
        head       = (head + 1) % n;
        --length;
        return true;
    }
    return false;
}

template<typename T, uint32_t n>
void RoundQueue<T, n>::push(const T&value) {
    if (length < n) {
        buffer[tail] = value;
        tail         = (tail + 1) % n;
        ++length;
    } else {
        head = (head + 1) % n;
        buffer[tail] = value;
        tail         = (tail + 1) % n;
    }
}
