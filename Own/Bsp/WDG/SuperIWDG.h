//
// Created by Administrator on 25-1-15.
//

#ifndef DM_42_SUPERIWDG_H
#define DM_42_SUPERIWDG_H

#define USING_IWDG 0
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
    [[gnu::always_inline]] inline void give();

    [[gnu::always_inline]] inline uint32_t read() {
        return hiwdg->Instance->KR;
    }

private:
    IWDG_HandleTypeDef* hiwdg;
};

[[gnu::always_inline]] inline void SuperIWDG::give() {
    HAL_IWDG_Refresh(hiwdg);
}
#endif

#endif //DM_42_SUPERIWDG_H
