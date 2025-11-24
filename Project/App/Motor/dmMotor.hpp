/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-25 21:02:39
 * @FilePath: /DM/Own/App/Motor/dmMotor.hpp
 * @Description: 
 */
//
// Created by Administrator on 24-10-16.
//
#pragma once

#define USING_DMMOTOR 0
#if USING_DMMOTOR == 1

#include "CAN/SuperCan.hpp"
#include "MyMath/MyMath.hpp"

class dmMotor {
public:

    dmMotor(uint16_t id) : ctrlId(id), state(0) {}

    void init(uint16_t ctrlId, SuperCan *superCan, float PMAX, float VMAX, float TMAX);

    void enable();

    void mit_ctrl(float pos, float vel, float tor, float kp, float kd);

    void disable();

    uint16_t ctrlId;

    uint8_t state;

    struct {
        float PMAX;
        float VMAX;
        float TMAX;
    } tmpDataMax;

private:
    SuperCan *superCan;


};

#include "Motor.hpp"
template<typename MOTOR>
void Motor<MOTOR>::get_feedback(uint8_t *data) requires std::same_as<MOTOR, dmMotor>{
    motor.state = data[0]>>4;
    int pos = 0;
    feed_back.raw_data.position = (data[1] << 8)|data[2];
    pos = (data[1] << 8)|data[2];
    feed_back.raw_data.speed = (data[3]<<4)|(data[4]>>4);
    feed_back.raw_data.current = ((data[4]&0xF)<<8)|data[5];
    feed_back.raw_data.temperature = data[6];

    //    Data.position = uint_to_float(RawData.position, -tmpDataMax.PMAX, tmpDataMax.PMAX, 16);
    feed_back.data.position = uint_to_float(pos, -motor.tmpDataMax.PMAX, motor.tmpDataMax.PMAX, 16);
    feed_back.data.speed = uint_to_float(feed_back.raw_data.speed, -motor.tmpDataMax.VMAX, motor.tmpDataMax.VMAX, 12);
    feed_back.data.current = uint_to_float(feed_back.raw_data.current, -motor.tmpDataMax.TMAX, motor.tmpDataMax.TMAX, 12);
    feed_back.data.temperature = data[7];
}

extern Motor<dmMotor> link1;


#endif //USING_DMMOTOR

