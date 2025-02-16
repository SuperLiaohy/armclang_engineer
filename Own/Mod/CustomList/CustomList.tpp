//
// Created by Administrator on 24-10-3.
//

#include "CustomList.h"


template <typename T>
CustomList<T>::CustomList() : head(nullptr), tail(nullptr), length(0) {}

template <typename T>
CustomList<T>::~CustomList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

template <typename T>
void CustomList<T>::push_back(const T& value) {
    Node* newNode = new Node(value);
    if (tail != nullptr) {
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
    ++length;
}

template <typename T>
void CustomList<T>::pop_back() {
    if (length == 0) return;

    if (head == tail) {
        delete head;
        head = tail = nullptr;
    } else {
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
    }
    --length;
}
template <typename T>
void CustomList<T>::remove(const T& value) {
    if (head == nullptr) return;

    if (head->data == value) {
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
        --length;
    } else {
        Node* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }
        if (current->next != nullptr) {
            Node* temp = current->next;
            current->next = current->next->next;
            if (temp == tail) {
                tail = current;
            }
            delete temp;
            --length;
        }
    }
}
template <typename T>
T& CustomList<T>::operator[](int index) {
    Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T>
const T& CustomList<T>::operator[](int index) const {
    Node* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template <typename T>
int CustomList<T>::size() const {
    return length;
}

template <typename T>
bool CustomList<T>::empty() const {
    return length == 0;
}