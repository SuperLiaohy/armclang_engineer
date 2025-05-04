//
// Created by Administrator on 25-5-4.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#ifdef __cplusplus
}
#endif
#include "cstdint"
template<typename T> class LinkedList {
public:
    LinkedList()
        : head(nullptr)
        , tail(nullptr)
        , length(0) {};
    ~LinkedList() = default;

    void push_back(const T& value) {
        if (head == nullptr) {
            head = new Node(value);
            tail = head;
            ++length;
        } else {
            tail->next = new Node(value);
            tail       = tail->next;
            ++length;
        }
    };
    void pop_back() {
        if (tail == nullptr) {
            return;
        } else if (tail == head) {
            delete tail;
            head   = nullptr;
            tail   = nullptr;
            length = 0;
        } else {
            Node* current = head;
            while (current->next != tail) { current = current->next; }
            delete tail;
            tail       = current;
            tail->next = nullptr;
            --length;
        }
    };
    void remove(const T& value) {
        if (head == nullptr) { return; }
        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            if (head == nullptr) tail = nullptr;
            --length;
            return;
        }
        Node* current = head->next;
        Node* last = head;
        while (current && current->data != value) {
            last = current;
            current = current->next;
        }
        if (current) {
            last->next = current->next;
            if (current == tail) tail = last;
            delete current;
            --length;
        }
    }
    T& operator[](int index) {
        Node* current = head;
        if (current == nullptr) { return nullptr; }
        for (int i = 0; i < index; ++i) { current = current->next; }
        return current->data;
    };
    const T& operator[](int index) const {
        Node* current = head;
        if (current == nullptr) { return nullptr; }
        for (int i = 0; i < index; ++i) { current = current->next; }
        return current->data;
    };
    [[nodiscard]] int size() const { return length; };
    [[nodiscard]] bool empty() const {
        if (head == nullptr) { return true; }
        return false;
    };

protected:
    struct Node {
        T data;
        Node* next;
        Node(const T& value)
            : data(value)
            , next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int length;

public:
    // 迭代器定义
    class Iterator {
    public:
        Iterator(Node* node)
            : current(node) {}

        T& operator*() { return current->data; }
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return current != other.current; }

    private:
        Node* current;
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};
