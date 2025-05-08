//
// Created by lhy on 2024/9/17.
//
#pragma once


#define USE_HAL_SELF_LIST 1

#if USE_HAL_SELF_LIST
#include "CustomList/CustomList.hpp"
#else
#include <algorithm>
#include "vector"
#endif

class Detect;
class TimDetect;

template<typename T>
class DetectManager {
public:
    DetectManager() = default;

    ~DetectManager() = default;

    void Register(T *detect);

    void Unregister(T *detect);

    void JudgeLost();

private:
#ifdef USE_HAL_SELF_LIST
    CustomList<T *, SeqList<T*, 30>> detects;
#else
    std::vector<Detect *> detects;
#endif
};


template<typename T>
DetectManager<T>& DetectManagerInstance() {
    static DetectManager<T> manager;
    return manager;
}



template<typename T>
void DetectManager<T>::Register(T *detect) {
    detects.push_back(detect);
}

template<typename T>
void DetectManager<T>::Unregister(T *detect) {
#ifdef USE_HAL_SELF_LIST
    detects.remove(detect);
#else
    detects.erase(std::remove(detects.begin(), detects.end(), detect), detects.end());
#endif
}

template<typename T>
void DetectManager<T>::JudgeLost() {
    for (auto detect: detects) {
        detect->JudgeLost();
    }
}
