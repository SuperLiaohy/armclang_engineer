//
// Created by Administrator on 25-5-4.
//

#pragma once
#include <cstdint>
template<typename T, uint8_t len> class SeqList {
public:
    SeqList()
        : index(0) {};
    ~SeqList() = default;

    void push_back(const T& value) {
        list[index] = value;
        ++index;
    };
    void pop_back() { --index; };
    void remove(const T& value) {
        for (int i = 0; i < index; ++i) {
            if (list[i] == value) {
                for (int j = i; j < index - 1; ++j) { list[j] = list[j + 1]; }
                --index;
                break;
            }
        }
    };
    T& operator[](int index) { return list[index]; };
    const T& operator[](int index) const { return list[index]; };
    [[nodiscard]] int size() const { return index; };
    [[nodiscard]] bool empty() const { return index == 0; };

protected:
    T list[len];
    uint8_t index;

public:
    // 迭代器定义
    class Iterator {
    public:
        Iterator(T* node)
            : current(node) {}

        T& operator*() { return *current; }
        Iterator& operator++() {
            current = current + 1;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return current != other.current; }

    private:
        T* current;
    };

    Iterator begin() { return Iterator(list); }
    Iterator end() { return Iterator(list + index); }
};
