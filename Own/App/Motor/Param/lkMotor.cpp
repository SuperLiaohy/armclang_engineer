//
// Created by Administrator on 2025/4/4.
//
#include "lkMotor.hpp"

void LKMotor::get_feedback(uint8_t* data) {
    ++rx_cnt;
    detect.update();
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
            total_position = feedback.total_position;
            float tmp = feedback.total_position;
            feedback.data.position = whileLimit<float>(tmp,0,360);
            feedback.data.last_position = feedback.data.position;
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
        default:
            break;
    }
}