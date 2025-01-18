//
// Created by Administrator on 24-11-2.
//

#ifndef DM_42_BUZZER_H
#define DM_42_BUZZER_H
#ifdef __cplusplus
extern "C" {
#endif
#include "tim.h"
#include "cmsis_os.h"
#ifdef __cplusplus
}
#endif
class Buzzer {
public:
    Buzzer(TIM_HandleTypeDef *_htim, uint16_t _Channel) : htim(_htim), Channel(_Channel) {
    }
    void Start();

    void SetFreq(uint16_t freq, uint16_t pulse = 200);

    void Stop();

    void StartMusic(uint16_t *music, uint16_t size);
private:
    void delay(uint32_t ms);
    TIM_HandleTypeDef *htim;
    uint16_t Channel;
};

inline void Buzzer::delay(uint32_t ms) {
    osDelay(ms);
}

inline void Buzzer::Start()  {
    HAL_TIM_PWM_Start(htim, Channel);
}

inline void Buzzer::SetFreq(uint16_t freq, uint16_t pulse) {
    if (freq<200 || freq>20000) {
        htim->Instance->ARR = 10000000 / 20000 - 1;
        htim->Init.Period = 10000000 / 20000 - 1;
        __HAL_TIM_SET_COMPARE(htim, Channel, 20);
        return;
    }
    htim->Instance->ARR = 10000000 / freq - 1;
    htim->Init.Period = 10000000 / freq - 1;
    __HAL_TIM_SET_COMPARE(htim, Channel, pulse);
}

inline void Buzzer::Stop() {
    HAL_TIM_PWM_Stop(htim, Channel);
}



extern uint16_t Romance[];
extern uint16_t error_music[];
extern Buzzer buzzer;
#endif //DM_42_BUZZER_H
