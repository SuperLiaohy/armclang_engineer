//
// Created by lhy on 2024/9/9.
//
#pragma once

#define USING_M3508 1
#if USING_M3508 == 1

#include "FeedBack.h"

class M3508 {
public:

    M3508(uint8_t ctrlId) : ctrlId(ctrlId) {}

    const uint16_t ctrlId;

    PID pid;

    void init(float p, float i, float d, float maxI, float maxOut, float gain);

    void enable(){};

    void disable(){};

    void readData(const uint8_t *data);

    float gain;
private:
    void CalcTotalPos();

};

#endif //USING_M3508
