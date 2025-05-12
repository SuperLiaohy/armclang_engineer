//
// Created by lhy on 2024/9/15.
//

#include "RingQueue.hpp"

template<typename T, uint32_t n>
bool RingQueue<T, n>::pop(T& value) {
    if (len() > 0) {
        value = buffer[head];
        head       = (head + 1) % n;
        return true;
    }
    return false;
}

template<typename T, uint32_t n>
void RingQueue<T, n>::push(const T&value) {
    if (len() < n - 1) {
        buffer[tail] = value;
        tail         = (tail + 1) % n;
    } else {
        head = (head + 1) % n;
        buffer[tail] = value;
        tail         = (tail + 1) % n;
    }
}
