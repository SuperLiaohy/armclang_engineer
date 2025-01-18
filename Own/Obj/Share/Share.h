//
// Created by lhy on 2024/9/15.
//

#ifndef NEW_PRJ_SHARE_H
#define NEW_PRJ_SHARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef __cplusplus
}
#endif



    struct Gimbal_share_Chassis{
        float relative_angle;
        int16_t now_angle;
    } ;


    extern Gimbal_share_Chassis gimbal_share_chassis;

#endif //NEW_PRJ_SHARE_H
