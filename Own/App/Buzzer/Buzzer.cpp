//
// Created by Administrator on 24-11-2.
//

#include "Buzzer.hpp"


// template<>
// void Buzzer::delay<Buzzer::delay_pl::OS>(uint32_t ms) {
//     osDelay(ms);
// }
// template<>
// void Buzzer::delay<Buzzer::delay_pl::HAL>(uint32_t ms) {
//     HAL_Delay(ms);
// }

void Buzzer::Start() {
    HAL_TIM_PWM_Start(htim, Channel);
}

void Buzzer::SetFreq(uint16_t freq, uint16_t pulse) {
    if (freq < 200 || freq > 20000) {
        htim->Instance->ARR = 10000000 / 20000 - 1;
        htim->Init.Period   = 10000000 / 20000 - 1;
        __HAL_TIM_SET_COMPARE(htim, Channel, 20);
        return;
    }
    htim->Instance->ARR = 10000000 / freq - 1;
    htim->Init.Period   = 10000000 / freq - 1;
    __HAL_TIM_SET_COMPARE(htim, Channel, pulse);
}

void Buzzer::Stop() {
    HAL_TIM_PWM_Stop(htim, Channel);
}



