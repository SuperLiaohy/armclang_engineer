//
// Created by lhy on 2024/9/9.
//

#include "GM6020.hpp"

#if USING_GM6020 == 1



void GM6020::init(float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
    float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut, float gain) {
    this->gain = gain;
    pid.set(inter_p, inter_i, inter_d, inter_maxI, inter_maxOut);
    doublePid.inter_pid.set(inter_p, inter_i, inter_d, inter_maxI, inter_maxOut);
    doublePid.extern_pid.set(extern_p, extern_i, extern_d, extern_maxI, extern_maxOut);
}

void GM6020::readData(const uint8_t *data) {

}

void GM6020::CalcTotalPos() {

}


float GM6020::SelfDoublePid::update(float extern_input, float extern_target, float inter_input) {
    float error = extern_target - extern_input;
    if (error > 4096) {
        error -= 8192;
    } else if (error < -4096) {
        error += 8192;
    }
    extern_input = extern_target - error;
    output = inter_pid.update(extern_pid.update(extern_target, extern_input), inter_input);
    return output;
}

#endif //USING_GM6020
