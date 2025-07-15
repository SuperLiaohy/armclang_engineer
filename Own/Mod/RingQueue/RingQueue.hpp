//
// Created by lhy on 2024/9/15.
//
#pragma once

#include <array>
#include <cstdint>

template<typename T, uint32_t n>
class RingQueue {
public:
    explicit RingQueue() : head(0), tail(0) {}

    bool pop(T &value);

    void push(const T &value);

    void clear() {
        head = 0;
        tail = 0;
    }


    [[nodiscard]] int32_t len() const {return (tail - head + n) % n;}

    [[nodiscard]] int32_t is_empty() const {return tail == head;}
    [[nodiscard]] int32_t is_full() const {return (tail + 1) % n == head;}

private:
    T buffer[n]{};

    int8_t head;
    int8_t tail;

};


