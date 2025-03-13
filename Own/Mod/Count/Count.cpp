//
// Created by liaohy on 25-3-12.
//
#include "CountManager.hpp"
CountManager& CountManagerInstance() {
    static CountManager instance;
    return instance;
}
