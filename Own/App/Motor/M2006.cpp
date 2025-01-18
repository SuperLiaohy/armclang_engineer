/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 18:16:36
 * @FilePath: /DM/Own/App/Motor/M2006.cpp
 * @Description: DJI 2006电机 使用的C610的FOC
 */
#include "M2006.h"

#if USING_M2006 == 1



void M2006::readData(const uint8_t *data) {

}

void M2006::CalcTotalPos() {

}

void M2006::init(uint16_t ctrlId, float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
                 float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut, float gain) {
    this->gain = gain;
    this->ctrlId = ctrlId;
    this->doublePid.inter_pid.set(inter_p, inter_i, inter_d, inter_maxI, inter_maxOut);
    this->doublePid.extern_pid.set(extern_p, extern_i, extern_d, extern_maxI, extern_maxOut);
}

#endif //USING_M2006


