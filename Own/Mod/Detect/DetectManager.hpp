//
// Created by lhy on 2024/9/17.
//
#pragma once


#define USE_HAL_SELF_LIST 1

#include "Detect.hpp"

#if USE_HAL_SELF_LIST
#include "CustomList/CustomList.hpp"
#else
#include <algorithm>
#include "vector"
#endif

class Detect;

class DetectManager {
public:
    DetectManager() = default;

    ~DetectManager() = default;

    void Register(Detect *detect);

    void Unregister(Detect *detect);

    void JudgeLost();

private:
#ifdef USE_HAL_SELF_LIST
    CustomList<Detect *> detects;
#else
    std::vector<Detect *> detects;
#endif
};


