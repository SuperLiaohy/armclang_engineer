//
// Created by liaohy on 24-11-17.
//
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"

#ifdef __cplusplus
}
#endif

#include "Pid/pid.hpp"
enum MEASURE_ABLE {
    MEASURE_ENABLE,
    MEASURE_DISABLE,
};
class Imu {

public:
    Imu(MEASURE_ABLE ifable = MEASURE_DISABLE): temprature_pid(0, 0, 0, 0, 0) {
        offset.restart_measure = ifable;
    };

    float roll;
    float pitch;
    float yaw;
    float temprature;

    /* 初始化 */
    void init();

    void mahony_start();

    void update();

    void update_raw_data();

    void update_offset(uint32_t cnt, uint8_t &flag);

    void update_temp_ctrl();

    uint8_t is_temp_const(uint32_t ticks);

    uint8_t is_temp_dead_condition(float err);

    struct {
        bool on;
        float correct[3];
        uint32_t target_times;
        MEASURE_ABLE restart_measure;
    } offset;

private:
    void ekf_update();

    void mahony_update();

    float gyro[3];
    float accel[3];

    PID temprature_pid;
    float temp_work;
    uint32_t temp_ticks;
    TIM_HandleTypeDef *pwm;
    uint32_t Channel;
};

extern Imu imu;
