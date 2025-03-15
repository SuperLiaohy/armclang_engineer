/*
 * @Author: SuperLiaohy liaohy123@outlook.com
 * @LastEditors: SuperLiaohy liaohy123@outlook.com
 * @LastEditTime: 2024-11-24 15:21:52
 * @FilePath: /DM/Own/App/Motor/lkMotor.hpp
 * @Description: 
 * Copyright (c) 2024 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#pragma once
#include "Motor.tpp"
#include "CAN/SuperCan.hpp"
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

class LKMotor {
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

    LKMotor(const uint16_t rx_id, const uint32_t range, const float ratio)
        : feedback()
        , rx_id(rx_id)
        , detect(1000)
        , precision_range(range)
        , reduction_ratio(ratio) {}

    inline void get_feedback(uint8_t* data);
private:
    Count rx_cnt;
};
inline void LKMotor::get_feedback(uint8_t* data) {
    ++rx_cnt;
    switch (data[0]) {
        case 0x80:
            // 关闭判断
            close_flag = 1;
            break;
        case 0x88:
            // 启动判断
            start_flag = 1;
            break;
        case 0xa0:
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xa6:
        case 0xa7:
        case 0xa8: {
            feedback.data.last_position   = feedback.data.position;
            feedback.raw_data.temperature = data[1];
            feedback.raw_data.current     = *(int16_t*)(&data[2]);
            feedback.raw_data.speed       = *(int16_t*)(&data[4]);
            feedback.raw_data.position    = *(int16_t*)(&data[6]);

            feedback.data.position    = feedback.raw_data.position * 360.f / precision_range;
            feedback.data.speed       = feedback.raw_data.speed;
            feedback.data.current     = feedback.raw_data.current * 32.f / 2000.f;
            feedback.data.temperature = feedback.raw_data.temperature;

            float dPos = feedback.data.position - feedback.data.last_position;
            if (dPos > 180) {
                dPos = dPos - 360;
            } else if (dPos < -180) {
                dPos = dPos + 360;
            }
            total_position += dPos;
        } break;
        case 0x90:
            feedback.encoder.encoder       = *(uint16_t*)(&data[2]) * 360.0 / precision_range;
            feedback.encoder.encoderRaw    = *(uint16_t*)(&data[4]) * 360.0 / precision_range;
            feedback.encoder.encoderOffset = *(uint16_t*)(&data[6]) * 360.0 / precision_range;
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

            feedback.total_position = static_cast<double>(total) / 100.f / reduction_ratio; // 0.01°/LSB
            offset_flag             = 1;
        } break;
        case 0x9b:
        case 0x9a:
            clear_flag                    = 1;
            feedback.raw_data.temperature = data[1];
            feedback.data.temperature     = data[1];
            feedback.err.voltage          = *(uint16_t*)(&data[3]) * 0.1;
            feedback.err.error            = data[7];
            break;
        case 0x9c:

            feedback.raw_data.temperature = data[1];
            feedback.raw_data.current     = *(int16_t*)(&data[2]);
            feedback.raw_data.speed       = *(int16_t*)(&data[4]);
            feedback.raw_data.position    = *(int16_t*)(&data[6]);

            feedback.data.position    = feedback.raw_data.position * 360.0 / precision_range;
            feedback.data.speed       = feedback.raw_data.speed;
            feedback.data.current     = feedback.raw_data.current * 32.f / 2000.f;
            feedback.data.temperature = feedback.raw_data.temperature;
            break;

        case 0x30:
            feedback.foc_pid.pos_p     = data[1];
            feedback.foc_pid.pos_i     = data[2];
            feedback.foc_pid.speed_p   = data[3];
            feedback.foc_pid.speed_i   = data[4];
            feedback.foc_pid.current_p = data[5];
            feedback.foc_pid.current_i = data[6];
            break;
    }
}
template<motor_param motor>
class LKControl {
public:
    LKControl(const uint16_t rx_id, uint32_t range, float ratio, SuperCan* canPlus)
        : m(rx_id, range, ratio)
        , tx_id(rx_id)
        , canPlus(canPlus) {}

    void set_position(float position, float speed = 100) {
        ++tx_cnt;
        totalposition2Control(speed, position * m.reduction_ratio);
    };

    void read_totalposition();

    bool get_feedback(uint16_t id, uint8_t* data) {
        if (id == m.rx_id) {
            m.get_feedback(data);
            return true;
        }
        return false;
    };

    void enable();

    void disable();

    void close();

    friend class RoboArm;
private:
    motor m;
    Count tx_cnt;
    uint16_t tx_id;

    SuperCan* canPlus;

    void voltageControl(int16_t target);

    void torqueControl(int16_t target);

    void speedControl(int32_t target);

    void totalposition1Control(int32_t target);

    void totalposition2Control(uint16_t speed, int32_t totalposition);

    void position1Control(uint8_t spin, uint32_t target);

    void position2Control(uint8_t spin, uint16_t speed, uint32_t target);

    void addposition1Control(int32_t target);

    void addposition2Control(uint16_t speed, int32_t target);

    void read_feedback();

    void read_encoder();

    // void read_totalposition();

    void readPid();

    void writeRAMPid();

    void writeROMPid();

    void read_error();

    void clear_error();
};




template<motor_param motor>
void LKControl<motor>::enable() {
    ++tx_cnt;
    uint8_t data[8] = {0x88, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::disable() {
    ++tx_cnt;
    uint8_t data[8] = {0x81, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::close() {
    ++tx_cnt;
    uint8_t data[8] = {0x80, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::voltageControl(int16_t target) {
    uint8_t data[8] = {0xa0, 0, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1), 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::torqueControl(int16_t target) {
    uint8_t data[8] = {0xa1, 0, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1), 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::speedControl(int32_t target) {
    uint8_t data[8] = {0xa2, 0, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2), *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::totalposition1Control(int32_t target) {
    uint8_t data[8] = {0xa3, 0, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2), *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::totalposition2Control(uint16_t speed, int32_t totalposition) {
    uint8_t data[8] = {0xa4, 0, *(uint8_t*)(&speed), *((uint8_t*)(&speed) + 1), *(uint8_t*)(&totalposition),
                       *((uint8_t*)(&totalposition) + 1), *((uint8_t*)(&totalposition) + 2),
                       *((uint8_t*)(&totalposition) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::position1Control(uint8_t spin, uint32_t target) {
    uint8_t data[8] = {0xa5, spin, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2), *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::position2Control(uint8_t spin, uint16_t speed, uint32_t target) {
    uint8_t data[8] = {0xa6, spin, *(uint8_t*)(&speed), *((uint8_t*)(&speed) + 1), *(uint8_t*)(&target),
                       *((uint8_t*)(&target) + 1), *((uint8_t*)(&target) + 2), *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::addposition1Control(int32_t target) {
    uint8_t data[8] = {0xa7, 0, 0, 0, *(uint8_t*)(&target), *((uint8_t*)(&target) + 1),
                       *((uint8_t*)(&target) + 2), *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::addposition2Control(uint16_t speed, int32_t target) {
    uint8_t data[8] = {0xa8, 0, *(uint8_t*)(&speed), *((uint8_t*)(&speed) + 1), *(uint8_t*)(&target),
                       *((uint8_t*)(&target) + 1), *((uint8_t*)(&target) + 2), *((uint8_t*)(&target) + 3)};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::read_feedback() {
    uint8_t data[8] = {0x9c, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::read_encoder() {
    uint8_t data[8] = {0x90, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::readPid() {
    uint8_t data[8] = {0x30, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::writeRAMPid() {
    uint8_t data[8] = {0x31, 0, m.feedback.foc_pid.pos_p, m.feedback.foc_pid.pos_i, m.feedback.foc_pid.speed_p, m.feedback.foc_pid.speed_i, m.feedback.foc_pid.current_p,
                       m.feedback.foc_pid.current_i};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::writeROMPid() {
    uint8_t data[8] = {0x32, 0, m.feedback.foc_pid.pos_p, m.feedback.foc_pid.pos_i, m.feedback.foc_pid.speed_p, m.feedback.foc_pid.speed_i, m.feedback.foc_pid.current_p,
                       m.feedback.foc_pid.current_i};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::read_error() {
    uint8_t data[8] = {0x9a, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::clear_error() {
    ++tx_cnt;
    uint8_t data[8] = {0x9b, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}

template<motor_param motor>
void LKControl<motor>::read_totalposition() {
    ++tx_cnt;
    uint8_t data[8] = {0x92, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(tx_id + motor::foc.TX_LOW_ID, data);
}


// class lkMotorBoard {
// public:
//     lkMotorBoard(uint16_t id)
//         : doublePid(Pid(), Pid())
//         , ctrlId(id) {};
//
//     void init(float inter_p, float inter_i, float inter_d, float inter_maxI, float inter_maxOut,
//               float extern_p, float extern_i, float extern_d, float extern_maxI, float extern_maxOut);
//
//     void readData(const uint8_t* data);
//
//     class SelfDoublePid : public DoublePid {
//     public:
//         SelfDoublePid(Pid inter_pid, Pid extern_pid)
//             : DoublePid(inter_pid, extern_pid) {}
//
//         float update(float extern_input, float extern_target, float inter_input) override;
//     } doublePid;
//
//     Pid pid;
//
//     const uint16_t ctrlId;
//
// private:
//     void CalcTotalPos();
// };

// template<typename MOTOR>
// void Motor<MOTOR>::get_feedback(uint8_t* data)
//     requires std::same_as<MOTOR, lkMotor>
// {
//     switch (data[0]) {
//         case 0x80:
//             // 关闭判断
//             motor.close_flag = 1;
//             break;
//         case 0x88:
//             // 启动判断
//             motor.start_flag = 1;
//             break;
//         case 0xa0:
//         case 0xa1:
//         case 0xa2:
//         case 0xa3:
//         case 0xa4:
//         case 0xa5:
//         case 0xa6:
//         case 0xa7:
//         case 0xa8: {
//             feed_back.data.lastPosition    = feed_back.data.position;
//             feed_back.raw_data.temperature = data[1];
//             feed_back.raw_data.current     = *(int16_t*)(&data[2]);
//             feed_back.raw_data.speed       = *(int16_t*)(&data[4]);
//             feed_back.raw_data.position    = *(int16_t*)(&data[6]);
//
//             feed_back.data.position    = feed_back.raw_data.position * 360.0 / feed_back.precision_range;
//             feed_back.data.speed       = feed_back.raw_data.speed;
//             feed_back.data.current     = feed_back.raw_data.current * 32.f / 2000.f;
//             feed_back.data.temperature = feed_back.raw_data.temperature;
//
//             //            int32_t dPos = feed_back.Data.position - feed_back.Data.lastPosition;
//             //            if (dPos > (static_cast<int32_t>(feed_back.precision_range) / 2)) {
//             //                dPos = dPos - feed_back.precision_range;
//             //            } else if (dPos < -(static_cast<int32_t>(feed_back.precision_range) / 2)) {
//             //                dPos = dPos + feed_back.precision_range;
//             //            }
//             //            feed_back.totalPosition += dPos;
//         } break;
//         case 0x90:
//             motor.encoder_data.encoder       = *(uint16_t*)(&data[2]) * 360.0 / feed_back.precision_range;
//             motor.encoder_data.encoderRaw    = *(uint16_t*)(&data[4]) * 360.0 / feed_back.precision_range;
//             motor.encoder_data.encoderOffset = *(uint16_t*)(&data[6]) * 360.0 / feed_back.precision_range;
//             break;
//         case 0x92: {
//             int64_t total = 0;
//             for (int i = 0; i < 7; i++) {
//                 total |= (int64_t)data[i + 1] << (i * 8); // 重组 7 字节
//             }
//
//             // 恢复符号：如果最高有效字节的符号位是负数，则扩展符号
//             if (data[7] == 0xFF) {
//                 total |= ((int64_t)0xFF << 56); // 将高 8 位填充为 0xFF，符号扩展
//             }
//
//             feed_back.totalPosition = static_cast<double>(total) / 100.f / motor.gain; // 0.01°/LSB
//             motor.offset_flag       = 1;
//         } break;
//         case 0x9b:
//         case 0x9a:
//             motor.clear_flag               = 1;
//             feed_back.raw_data.temperature = data[1];
//             feed_back.data.temperature     = data[1];
//             motor.voltage                  = *(uint16_t*)(&data[3]) * 0.1;
//             motor.error                    = data[7];
//             break;
//         case 0x9c:
//
//             feed_back.raw_data.temperature = data[1];
//             feed_back.raw_data.current     = *(int16_t*)(&data[2]);
//             feed_back.raw_data.speed       = *(int16_t*)(&data[4]);
//             feed_back.raw_data.position    = *(int16_t*)(&data[6]);
//
//             feed_back.data.position    = feed_back.raw_data.position * 360.0 / feed_back.precision_range;
//             feed_back.data.speed       = feed_back.raw_data.speed;
//             feed_back.data.current     = feed_back.raw_data.current * 32.f / 2000.f;
//             feed_back.data.temperature = feed_back.raw_data.temperature;
//             break;
//
//         case 0x30:
//             motor.foc_pid.pos_p     = data[1];
//             motor.foc_pid.pos_i     = data[2];
//             motor.foc_pid.speed_p   = data[3];
//             motor.foc_pid.speed_i   = data[4];
//             motor.foc_pid.current_p = data[5];
//             motor.foc_pid.current_i = data[6];
//             break;
//     }
// }

// template<typename MOTOR>
// void Motor<MOTOR>::get_feedback(uint8_t* data)
//     requires std::same_as<MOTOR, lkMotorBoard>
// {
//     feed_back.data.lastPosition    = feed_back.data.position;
//     feed_back.raw_data.temperature = data[1];
//     feed_back.raw_data.current     = *(int16_t*)(&data[2]);
//     feed_back.raw_data.speed       = *(int16_t*)(&data[4]);
//     feed_back.raw_data.position    = *(int16_t*)(&data[6]);
//
//     feed_back.data.position    = feed_back.raw_data.position * 360.0 / feed_back.precision_range;
//     feed_back.data.speed       = feed_back.raw_data.speed;
//     feed_back.data.current     = feed_back.raw_data.current * 32.f / 2000.f;
//     feed_back.data.temperature = feed_back.raw_data.temperature;
//
//     int32_t dPos = feed_back.data.position - feed_back.data.lastPosition;
//     if (dPos > 180) {
//         dPos = dPos - 360;
//     } else if (dPos < -180) {
//         dPos = dPos + 360;
//     }
//     feed_back.totalPosition += dPos;
// }
//
// extern Motor<lkMotorBoard> test_motor;

#endif //USING_LKMOTOR
