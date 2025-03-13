//
// Created by Administrator on 2025/3/8.
//

#pragma once

#include "CustomList/CustomList.hpp"

class Count;

class CountManager {
public:
    void register_item(Count* count) {
        list.push_back(count);
    };

    void unregister_item(Count* count) {
        list.remove(count);
    }

    void mark();

private:
    CustomList<Count*> list;
};
