//
// Created by Administrator on 24-10-22.
//

#include "lkMotor.hpp"

// #include "Motor.tpp"

#if USING_LKMOTOR == 1





// void lkMotorBoard::init(float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
//                         float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut) {
//     //    this->ctrlId = ctrlId;
//     pid.set(inter_p, inter_i, inter_d, inter_maxI, inter_maxOut);
//     doublePid.inter_pid.set(inter_p, inter_i, inter_d, inter_maxI, inter_maxOut);
//     doublePid.extern_pid.set(extern_p, extern_i, extern_d, extern_maxI, extern_maxOut);
// }
//
// void lkMotorBoard::readData(const uint8_t* data) {
// }
//
// void lkMotorBoard::CalcTotalPos() {
// }
// float lkMotorBoard::SelfDoublePid::update(float extern_input, float extern_target, float inter_input) {
//     float error = extern_target - extern_input;
//     if (error > 32767) {
//         error -= 32767;
//     } else if (error < -32767) {
//         error += 32767;
//     }
//     extern_input = extern_target - error;
//     output       = inter_pid.update(extern_pid.update(extern_target, extern_input), inter_input);
//     return output;
// }
#endif //USING_LKMOTOR
