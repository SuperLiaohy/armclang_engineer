/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 17:30:31
 * @FilePath: /DM/Own/App/Motor/M2006.h
 * @Description: DJI 2006电机 使用的C610的FOC
 */
#ifndef DM_42_M2006_H
#define DM_42_M2006_H

#define USING_M2006 1
#if USING_M2006 == 1

#include "FeedBack.h"

class M2006 {
public:

    M2006(uint16_t id) : doublePid(PID(), PID()), ctrlId(id) {};

    void init(float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
              float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut, float gain);

    void readData(const uint8_t *data);

    class SelfDoublePid : public DoublePid {
    public:
        SelfDoublePid(PID inter_pid, PID extern_pid) : DoublePid(inter_pid, extern_pid) {}

//        float update(float extern_input, float extern_target, float inter_input) override;
    } doublePid;

    PID pid;

    const uint16_t ctrlId;
    float gain;
private:
    void CalcTotalPos();
};

#endif //USING_M2006
#endif
