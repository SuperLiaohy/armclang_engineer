//
// Created by Administrator on 2025/3/8.
//

#pragma once
#include "CountManager.hpp"
#include <cstdint>
CountManager& CountManagerInstance();
class Count {
public:
    Count() { CountManagerInstance().register_item(this); };

    ~Count() { CountManagerInstance().unregister_item(this); };

    void increment() { ++cnt; }

    void mark() {
        max_cnt = cnt;
        cnt     = 0;
    }

    void operator++() { ++cnt; }

private:
    uint32_t cnt;
    uint32_t max_cnt;
};
