//
// Created by lhy on 2024/9/17.
//

#include "Detect.hpp"

Detect::~Detect() { DetectManagerInstance<Detect>().Unregister(this); }

Detect::Detect(const uint32_t maxInterval, Fun lostFun, Fun recoverFun)
    : isLost(false)
    , lostFun(lostFun)
    , recoverFun(recoverFun)
    , maxInterval(maxInterval)
    , lastReceiveTime(0) {
        DetectManagerInstance<Detect>().Register(this);
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

TimDetect::~TimDetect() { DetectManagerInstance<TimDetect>().Unregister(this); }

TimDetect::TimDetect(const uint32_t maxInterval, Fun lostFun, Fun recoverFun)
    : isLost(false)
    , lostFun(lostFun)
    , recoverFun(recoverFun)
    , maxInterval(maxInterval)
    , lastReceiveTime(0) {
        DetectManagerInstance<TimDetect>().Register(this);
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
