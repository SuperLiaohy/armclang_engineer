/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 15:21:52
 * @FilePath: /DM/Own/App/Motor/lkMotor.hpp
 * @Description: 
 * Copyright (c) 2024 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#pragma once
#include "../Motor.tpp"
#include "CAN/SuperCan.hpp"

#include <MyMath/MyMath.hpp>
#define USING_LKMOTOR 1
#if USING_LKMOTOR == 1

#define USING_SINGLE 1
struct foc_pid_t {
    uint8_t pos_p;
    uint8_t pos_i;
    uint8_t speed_p;
    uint8_t speed_i;
    uint8_t current_p;
    uint8_t current_i;
};
struct encoder_t {
    float encoder;
    float encoderRaw;
    float encoderOffset;
};

struct err_t {
    uint8_t error;
    uint8_t voltage;
};

class LKSingleMotor {
public:
    using Feedback = struct Feedback : public DefaultFeedback {
        encoder_t encoder;
        foc_pid_t foc_pid;
        err_t err;
    };

public:
    Feedback feedback;
    uint16_t rx_id;
    Detect detect;
    uint32_t precision_range;
    float reduction_ratio;
    float total_position;

    uint8_t clear_flag;
    uint8_t close_flag;
    uint8_t start_flag;
    uint8_t offset_flag;

    static constexpr FOC foc = {0x140, 0x140, 0x140};

    LKSingleMotor(const uint16_t rx_id, const uint32_t range, const float ratio)
        : feedback()
        , rx_id(rx_id)
        , detect(1000)
        , precision_range(range)
        , reduction_ratio(ratio)
        , total_position(0)
        , clear_flag(0)
        , close_flag(0)
        , start_flag(0)
        , offset_flag(0) {}

    void get_feedback(uint8_t* data);

private:
    Count rx_cnt;
};



#endif //USING_LKMOTOR
