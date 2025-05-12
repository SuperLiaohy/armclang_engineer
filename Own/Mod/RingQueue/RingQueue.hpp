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

    bool pop(T& value);

    void push(const T&value);

    void clear() {
        head = 0;
        tail = 0;
    }

    [[nodiscard]] int32_t len() const {
        return (tail - head + n) % n;
    }

private:
    T buffer[n]{};

    int8_t head;
    int8_t tail;

};

template<typename T, uint32_t n>
class RoundBuffer {
public:
    explicit RoundBuffer() : head(0), tail(0) {}

    void pop(T& value);

    T pop();

    void push(const T&value);

    template<uint32_t m>
    void push(const std::array<T, m>& value);

    void clear() {
        head = 0;
        tail = 0;
    }
private:
    T buffer[n]{};

    int8_t head;
    int8_t tail;

};

template<typename T, uint32_t n>
void RoundBuffer<T, n>::pop(T& value) {
    value = buffer[head];
    head = (head + 1) % n;
}
template<typename T, uint32_t n>
T RoundBuffer<T, n>::pop() {
    auto value = buffer[head];
    head = (head + 1) % n;
    return value;
}

template<typename T, uint32_t n>
void RoundBuffer<T, n>::push(const T& value) {
    buffer[tail] = value;
    tail = (tail + 1) % n;
}

template<typename T, uint32_t n>
template<uint32_t m>
void RoundBuffer<T, n>::push(const std::array<T, m>& value) {
    for (auto element: value) {
        push(element);
    }
}

