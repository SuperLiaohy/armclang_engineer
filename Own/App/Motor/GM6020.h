//
// Created by lhy on 2024/9/9.
//

#ifndef NONE_PRJ_GM6020_H
#define NONE_PRJ_GM6020_H

#define USING_GM6020 1
#if USING_GM6020 == 1

#include "FeedBack.h"
#include "Motor.h"

class GM6020 {
public:

    GM6020(uint16_t id) : doublePid(PID(), PID()), ctrlId(id) {};

    void init(float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
        float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut, float gain);

    void readData(const uint8_t *data);

    class SelfDoublePid : public DoublePid {
    public:
        SelfDoublePid(PID inter_pid, PID extern_pid) : DoublePid(inter_pid, extern_pid) {}

        float update(float extern_input, float extern_target, float inter_input) override;
    } doublePid;

    PID pid;

    const uint16_t ctrlId;

    float gain;
private:

    void CalcTotalPos();
};

extern Motor<GM6020> gm6020;


#endif //USING_GM6020
#endif //NONE_PRJ_GM6020_H
