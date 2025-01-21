/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @Date: 2024-11-24 12:48:55
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-25 21:17:01
 * @FilePath: /DM/Own/Obj/RoboArm/RoboArm.h
 * @Description: 机械臂类 瓴控和大疆的电机: (5010 8016 8016)三个关节  (4005 2006 2006)末端机构
 */

#ifndef DM_42_ROBOARM_H
#define DM_42_ROBOARM_H

#include "roboarm_dep.h"

extern int32_t left_dPos;
extern int32_t right_dPos;

class Interact;

class RoboArm {
public:
    RoboArm(
            uint32_t id1, uint32_t range1,
            uint32_t id2, uint32_t range2,
            uint32_t id3, uint32_t range3,
            uint32_t id4, uint32_t range4,
            float gain, uint32_t maxInterval,
            uint32_t id5, uint32_t range5,
            uint32_t id6, uint32_t range6,
            roboarm_dep::offset &&offset)
            : diff(gain, maxInterval, id5, range5, id6, range6), joint1(id1, range1), joint2(id2, range2),
              joint3(id3, range3), joint4(id4, range4), offset{offset} {};

    void updateArm();

    void enable();

    void disable();

    void close();

    /* 正运动学解 顺序x y z */
    float pos[3] = {0, 0, 0};
    /* 逆运动学解 顺序q1 q2 q3 并且有四种*/
    float q[3] = {};

    void init_offset(Interact &interaction);

    void fkine();

    bool ikine(const float *pos);

    roboarm_dep::Differentiator diff;

    Motor<lkMotor> joint1;
    Motor<lkMotor> joint2;
    Motor<lkMotor> joint3;
    Motor<lkMotor> joint4;

    roboarm_dep::offset offset{};

    roboarm_dep::target target{};

    void load_target(Interact &inter);
    void load_diff_target(Interact &inter);
    roboarm_dep::real_relative_pos real_relative_pos{};

    void update_relative_pos();

private:
};

extern RoboArm roboArm;

#endif //DM_42_ROBOARM_H
