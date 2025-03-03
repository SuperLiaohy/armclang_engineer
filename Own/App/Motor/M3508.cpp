//
// Created by lhy on 2024/9/9.
//

#include "M3508.hpp"

#if USING_M3508 == 1



template<motor_param motor>
void SpeedPidControl<motor>::init(float p, float i, float d, float maxI, float maxOut, float gain) {
    this->m.reduction_ratio = gain;
    this->speed.set(p, i, d, maxI, maxOut);
}



#endif //USING_M3508
