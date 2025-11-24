//
// Created by Administrator on 24-10-3.
//
#pragma once
#include "LinkedList.hpp"
#include "SeqList.hpp"
template <typename T, typename Vec = LinkedList<T>>
class CustomList : public Vec{
public:
    CustomList() : Vec() {};
    ~CustomList() = default;
};

