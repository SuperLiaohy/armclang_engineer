//
// Created by Administrator on 24-10-16.
//

#include "dmMotor.h"
#include "MyMath//MyMath.h"

#if USING_DMMOTOR == 1


void dmMotor::init(uint16_t ctrlId, SuperCan *superCan, float PMAX, float VMAX, float TMAX) {
    this->ctrlId = ctrlId;
    this->superCan = superCan;
    this->tmpDataMax.PMAX = PMAX;
    this->tmpDataMax.VMAX = VMAX;
    this->tmpDataMax.TMAX = TMAX;
}

void dmMotor::enable() {
    uint8_t data[8];

    data[0] = 0xFF;
    data[1] = 0xFF;
    data[2] = 0xFF;
    data[3] = 0xFF;
    data[4] = 0xFF;
    data[5] = 0xFF;
    data[6] = 0xFF;
    data[7] = 0xFC;

    superCan->send(ctrlId, data);
}


void dmMotor::mit_ctrl(float pos, float vel, float tor, float kp, float kd) {
    uint8_t data[8];
    uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;

    pos_tmp = float_to_uint(pos, -tmpDataMax.PMAX, tmpDataMax.PMAX, 16);
    vel_tmp = float_to_uint(vel, -tmpDataMax.VMAX, tmpDataMax.VMAX, 12);
    tor_tmp = float_to_uint(tor, -tmpDataMax.TMAX, tmpDataMax.TMAX, 12);
    kp_tmp  = float_to_uint(kp,  0, 500, 12);
    kd_tmp  = float_to_uint(kd,  0, 5, 12);

    data[0] = (pos_tmp >> 8);
    data[1] = pos_tmp;
    data[2] = (vel_tmp >> 4);
    data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
    data[4] = kp_tmp;
    data[5] = (kd_tmp >> 4);
    data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
    data[7] = tor_tmp;

    superCan->send(ctrlId, data);
}



void dmMotor::disable() {

}





#endif //USING_DMMOTOR
