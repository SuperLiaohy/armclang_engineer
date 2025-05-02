//
// Created by lhy on 2024/9/17.
//

#include "Detect.hpp"


DetectManager<Detect>& DetectManagerInstance() {
    static DetectManager<Detect> manager;
    return manager;
}


DetectManager<TimDetect>& TimDetectManagerInstance() {
    static DetectManager<TimDetect> tim_manager;
    return tim_manager;
}



Detect::~Detect() { DetectManagerInstance().Unregister(this); }

Detect::Detect(const uint32_t maxInterval, Fun lostFun, Fun recoverFun)
    : isLost(false)
    , lostFun(lostFun)
    , recoverFun(recoverFun)
    , maxInterval(maxInterval)
    , lastReceiveTime(0) {
        DetectManagerInstance().Register(this);
}

void Detect::update() { lastReceiveTime = getSysTime(); }

void Detect::JudgeLost() {
    uint32_t presentTime = getSysTime();
    if (presentTime - lastReceiveTime > maxInterval) {
        if (!isLost) {
            if (lostFun != nullptr) lostFun();
            isLost = 1;
        }
    } else {
        if (isLost) {
            if (recoverFun != nullptr) recoverFun();
            isLost = 0;
        }
    }
}

TimDetect::~TimDetect() { TimDetectManagerInstance().Unregister(this); }

TimDetect::TimDetect(const uint32_t maxInterval, Fun lostFun, Fun recoverFun)
    : isLost(false)
    , lostFun(lostFun)
    , recoverFun(recoverFun)
    , maxInterval(maxInterval)
    , lastReceiveTime(0) {
        TimDetectManagerInstance().Register(this);
}

void TimDetect::update() { lastReceiveTime = getSysTime(); }

void TimDetect::JudgeLost() {
    uint32_t presentTime = getSysTime();
    if (presentTime - lastReceiveTime > maxInterval) {
        if (!isLost) {
            if (lostFun != nullptr) lostFun();
            isLost = 1;
        }
    } else {
        if (isLost) {
            if (recoverFun != nullptr) recoverFun();
            isLost = 0;
        }
    }
}
