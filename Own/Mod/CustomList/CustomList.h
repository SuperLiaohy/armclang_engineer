//
// Created by Administrator on 24-10-3.
//

#ifndef OWN_CUSTOMLIST_H
#define OWN_CUSTOMLIST_H

template <typename T>
class CustomList {
public:
    CustomList();
    ~CustomList();

    void push_back(const T& value);
    void pop_back();
    void remove(const T& value);
    T& operator[](int index);
    const T& operator[](int index) const;
    int size() const;
    bool empty() const;

private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int length;

public:
    // 迭代器定义
    class Iterator {
    public:
        Iterator(Node* node) : current(node) {}

        T& operator*() { return current->data; }
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

    private:
        Node* current;
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

};

#include "CustomList/CustomList.tpp"

#endif //OWN_CUSTOMLIST_H
