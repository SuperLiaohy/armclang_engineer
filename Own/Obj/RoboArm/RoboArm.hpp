/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @Date: 2024-11-24 12:48:55
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-25 21:17:01
 * @FilePath: /DM/Own/Obj/RoboArm/RoboArm.hpp
 * @Description: 机械臂类 瓴控和大疆的电机: (5010 8016 8016)三个关节  (4005 2006 2006)末端机构
 */
#pragma once
#include "roboarm_dep.hpp"

#include <array>
extern int32_t left_dPos;
extern int32_t right_dPos;

class Interact;

class RoboArm {
public:
    RoboArm(SuperCan* canPlus, uint32_t id1, uint32_t range1, const float ratio1, uint32_t id2_internal,
            uint32_t range2_internal, const float ratio2_internal, uint32_t id2_external, uint32_t range2_external,
            const float ratio2_external, uint32_t id3, uint32_t range3, const float ratio3, uint32_t id4,
            uint32_t range4, const float ratio4, float gain, uint32_t id5, const Pid& left_pos_pid,
            const Pid& left_speed_pid, uint32_t id6, const Pid& right_pos_pid, const Pid& right_speed_pid,I2C_HandleTypeDef* hi2c,
            roboarm_dep::offset&& offset)
        : diff(gain, id5, left_pos_pid, left_speed_pid, id6, right_pos_pid, right_speed_pid, hi2c)
        , joint1(canPlus, id1, range1, ratio1)
        , joint2 {Motor<LKMotorSingle>(canPlus, id2_internal, range2_internal, ratio2_internal),
                  Motor<LKMotorSingle>(canPlus, id2_external, range2_external, ratio2_external)}
        , joint3(canPlus, id3, range3, ratio3)
        , joint4(canPlus, id4, range4, ratio4)
        , offset {offset} {};

    void enable();

    void disable();

    void close();

    /* 正运动学解 顺序x y z */
    float pos[3] = {0, 0, 0};
    /* 逆运动学解 顺序q1 q2 q3 并且有四种*/
    float q[3] = {};

    void init_offset(std::array<float, 6>& joint);

    void fkine(std::array<float, 3>& xyz);

    bool ikine(const std::array<float, 3>& pos);

    roboarm_dep::Differentiator diff;

    Motor<LKMotorSingle> joint1;

    struct {
        Motor<LKMotorSingle> internal;
        Motor<LKMotorSingle> external;
    } joint2;
    Motor<LKMotorSingle> joint3;
    Motor<LKMotorSingle> joint4;

    roboarm_dep::offset offset {};
    roboarm_dep::target target {};
    std::array<float, 4> target_speed {roboarm_dep::default_speed};

    void load_target(const std::array<float, 6>& joint, std::array<Slope, 3>& Slope);

    std::array<float, 6> relative_pos;
    void update_relative_pos();

private:
};

extern RoboArm roboArm;
