//
// Created by lhy on 2024/9/9.
//

#include "M3508.h"

#if USING_M3508 == 1



void M3508::init(uint16_t ctrlId, float p, float i, float d, float maxI, float maxOut, float gain) {
    this->gain = gain;
    this->ctrlId = ctrlId;
    this->pid.set(p, i, d, maxI, maxOut);
}

void M3508::readData(const uint8_t *data) {

}

void M3508::CalcTotalPos() {

}

#endif //USING_M3508
