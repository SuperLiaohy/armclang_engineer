/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 15:21:52
 * @FilePath: /DM/Own/App/Motor/lkMotor.h
 * @Description: 
 * Copyright (c) 2024 by ${git_name} email: ${git_email}, All Rights Reserved.
 */

#ifndef DM_42_LKMOTOR_H
#define DM_42_LKMOTOR_H

#define USING_LKMOTOR 1
#if USING_LKMOTOR == 1

#include "FeedBack.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"

#ifdef __cplusplus
}
#endif


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
class lkMotor {
public:
    //    lkMotor(PID inter_pid, PID extern_pid, SuperCan *superCan)
    //            : doublePid(inter_pid, extern_pid), superCan(superCan) {pid = inter_pid;}
    lkMotor(uint16_t id) : id(id) {
    };
    uint8_t voltage = 0;
    uint8_t error = 0;

    foc_pid_t foc_pid{};

    encoder_t encoder_data{};

    uint8_t offset_flag = 0;

    float gain;

    void enable();

    void disable();

    void close();

    void init(SuperCan *canPlus, float gain) {
        superCan = canPlus;
        this->gain = gain;
    };

    void voltageControl(int16_t target);

    void torqueControl(int16_t target);

    void speedControl(int32_t target);

    void totalposition1Control(int32_t target);

    void totalposition2Control(uint16_t speed, int32_t totalposition);

    void position1Control(uint8_t spin, uint32_t target);

    void position2Control(uint8_t spin, uint16_t speed, uint32_t target);

    //    void position2Feedback(const uint8_t* data);

    void addposition1Control(int32_t target);

    void addposition2Control(uint16_t speed, int32_t target);

    void read_feedback();

    void read_encoder();

    void read_totalposition();

    void readPid();

    void writeRAMPid();

    void writeROMPid();

    void read_error();

private:
    uint8_t id;
    SuperCan *superCan;
};


class lkMotorBoard {
public:

    lkMotorBoard(uint16_t id) :
            doublePid(PID(), PID()),
            ctrlId(id) {};

    void init(float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
              float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut);

    void readData(const uint8_t *data);

    class SelfDoublePid : public DoublePid {
    public:
        SelfDoublePid(PID inter_pid, PID extern_pid) : DoublePid(inter_pid, extern_pid) {}

        float update(float extern_input, float extern_target, float inter_input) override;
    } doublePid;

    PID pid;

    const uint16_t ctrlId;
private:

    void CalcTotalPos();
};


#include "Motor.h"

template<typename MOTOR>
void Motor<MOTOR>::readData(uint8_t *data) requires std::same_as<MOTOR, lkMotor> {
    switch (data[0]) {
        case 0xa0:
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xa6:
        case 0xa7:
        case 0xa8: {
            feed_back.Data.lastPosition = feed_back.Data.position;
            feed_back.RawData.temperature = data[1];
            feed_back.RawData.current = *(int16_t *) (&data[2]);
            feed_back.RawData.speed = *(int16_t *) (&data[4]);
            feed_back.RawData.position = *(int16_t *) (&data[6]);

            feed_back.Data.position = feed_back.RawData.position * 360.0 / feed_back.precision_range;
            feed_back.Data.speed = feed_back.RawData.speed;
            feed_back.Data.current = feed_back.RawData.current * 32.f / 2000.f;
            feed_back.Data.temperature = feed_back.RawData.temperature;

//            int32_t dPos = feed_back.Data.position - feed_back.Data.lastPosition;
//            if (dPos > (static_cast<int32_t>(feed_back.precision_range) / 2)) {
//                dPos = dPos - feed_back.precision_range;
//            } else if (dPos < -(static_cast<int32_t>(feed_back.precision_range) / 2)) {
//                dPos = dPos + feed_back.precision_range;
//            }
//            feed_back.totalPosition += dPos;
        }
            break;
        case 0x90:
            motor.encoder_data.encoder = *(uint16_t *) (&data[2]) * 360.0 / feed_back.precision_range;
            motor.encoder_data.encoderRaw = *(uint16_t *) (&data[4]) * 360.0 / feed_back.precision_range;
            motor.encoder_data.encoderOffset = *(uint16_t *) (&data[6]) * 360.0 / feed_back.precision_range;
            break;
        case 0x92: {
            int64_t total = 0;
            for (int i = 0; i < 7; i++) {
                total |= (int64_t)data[i + 1] << (i * 8); // 重组 7 字节
            }

            // 恢复符号：如果最高有效字节的符号位是负数，则扩展符号
            if (data[7] == 0xFF) {
                total |= ((int64_t)0xFF << 56); // 将高 8 位填充为 0xFF，符号扩展
            }

            feed_back.totalPosition = static_cast<double>(total)/100.f/motor.gain;    // 0.01°/LSB
            motor.offset_flag = 1;
        }
//            feed_back.totalPosition = double(*(int64_t *) (&data[1]));
            break;
        case 0x9a:
            feed_back.RawData.temperature = data[1];
            feed_back.Data.temperature = data[1];
            motor.voltage = *(uint16_t*)(&data[3]) * 0.1;
            motor.error = data[7];
            break;
        case 0x9c:

            feed_back.RawData.temperature = data[1];
            feed_back.RawData.current = *(int16_t *) (&data[2]);
            feed_back.RawData.speed = *(int16_t *) (&data[4]);
            feed_back.RawData.position = *(int16_t *) (&data[6]);

            feed_back.Data.position = feed_back.RawData.position * 360.0 / feed_back.precision_range;
            feed_back.Data.speed = feed_back.RawData.speed;
            feed_back.Data.current = feed_back.RawData.current * 32.f / 2000.f;
            feed_back.Data.temperature = feed_back.RawData.temperature;
            break;

        case 0x30:
            motor.foc_pid.pos_p = data[1];
            motor.foc_pid.pos_i = data[2];
            motor.foc_pid.speed_p = data[3];
            motor.foc_pid.speed_i = data[4];
            motor.foc_pid.current_p = data[5];
            motor.foc_pid.current_i = data[6];
            break;
    }
}

template<typename MOTOR>
void Motor<MOTOR>::readData(uint8_t *data) requires std::same_as<MOTOR, lkMotorBoard> {
    feed_back.Data.lastPosition = feed_back.Data.position;
    feed_back.RawData.temperature = data[1];
    feed_back.RawData.current = *(int16_t *) (&data[2]);
    feed_back.RawData.speed = *(int16_t *) (&data[4]);
    feed_back.RawData.position = *(int16_t *) (&data[6]);

    feed_back.Data.position = feed_back.RawData.position * 360.0 / feed_back.precision_range;
    feed_back.Data.speed = feed_back.RawData.speed;
    feed_back.Data.current = feed_back.RawData.current * 32.f / 2000.f;
    feed_back.Data.temperature = feed_back.RawData.temperature;

    int32_t dPos = feed_back.Data.position - feed_back.Data.lastPosition;
    if (dPos > 180) {
        dPos = dPos - 360;
    } else if (dPos < -180) {
        dPos = dPos + 360;
    }
    feed_back.totalPosition += dPos;
}


extern Motor<lkMotorBoard> test_motor;

#endif //USING_LKMOTOR
#endif //DM_42_LKMOTOR_H
