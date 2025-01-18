/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-25 21:02:39
 * @FilePath: /DM/Own/App/Motor/dmMotor.h
 * @Description: 
 */
//
// Created by Administrator on 24-10-16.
//

#ifndef DM_42_DMMOTOR_H
#define DM_42_DMMOTOR_H

#define USING_DMMOTOR 1
#if USING_DMMOTOR == 1

#include "CAN/SuperCan.h"
#include "MyMath/MyMath.h"

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

#include "Motor.h"
template<typename MOTOR>
void Motor<MOTOR>::readData(uint8_t *data) requires std::same_as<MOTOR, dmMotor>{
    motor.state = data[0]>>4;
    int pos = 0;
    feed_back.RawData.position = (data[1] << 8)|data[2];
    pos = (data[1] << 8)|data[2];
    feed_back.RawData.speed = (data[3]<<4)|(data[4]>>4);
    feed_back.RawData.current = ((data[4]&0xF)<<8)|data[5];
    feed_back.RawData.temperature = data[6];

    //    Data.position = uint_to_float(RawData.position, -tmpDataMax.PMAX, tmpDataMax.PMAX, 16);
    feed_back.Data.position = uint_to_float(pos, -motor.tmpDataMax.PMAX, motor.tmpDataMax.PMAX, 16);
    feed_back.Data.speed = uint_to_float(feed_back.RawData.speed, -motor.tmpDataMax.VMAX, motor.tmpDataMax.VMAX, 12);
    feed_back.Data.current = uint_to_float(feed_back.RawData.current, -motor.tmpDataMax.TMAX, motor.tmpDataMax.TMAX, 12);
    feed_back.Data.temperature = data[7];
}

extern Motor<dmMotor> link1;


#endif //USING_DMMOTOR
#endif //DM_42_DMMOTOR_H
