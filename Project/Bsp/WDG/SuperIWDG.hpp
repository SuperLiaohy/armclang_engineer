//
// Created by Administrator on 25-1-15.
//
#pragma once

#define USING_IWDG 1
#if USING_IWDG == 1
    #ifdef __cplusplus
extern "C" {
    #endif

    #include "iwdg.h"

    #ifdef __cplusplus
}
    #endif

class SuperIWDG {
public:
    [[gnu::always_inline]] inline void update();

    [[gnu::always_inline]] inline uint32_t read() {
        return hiwdg->Instance->KR;
    }

    static SuperIWDG& instance() {
        static auto instance = SuperIWDG(&hiwdg1);
        return instance;
    }
private:
    SuperIWDG(IWDG_HandleTypeDef* hiwdg) : hiwdg(hiwdg) {};
    IWDG_HandleTypeDef* hiwdg;
};

[[gnu::always_inline]] inline void SuperIWDG::update() {
    HAL_IWDG_Refresh(hiwdg);
}
#endif

