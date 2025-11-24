//
// Created by liaohy on 25-3-12.
//

#include "Count/Count.hpp"

CountManager& CountManagerInstance() {
    static CountManager instance;
    return instance;
}
void CountManager::mark() {
    for (const auto item: list) { item->mark(); }
}