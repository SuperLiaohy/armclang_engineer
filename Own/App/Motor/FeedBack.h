//
// Created by lhy on 2024/9/9.
//

#ifndef NONE_PRJ_MOTOR_H
#define NONE_PRJ_MOTOR_H

#include "CAN/SuperCan.h"
#include "Pid/DoublePid.h"
#include "Detect/Detect.h"

struct FOC {
    uint16_t RX_ID;
    uint16_t TX_LOW_ID;
    uint16_t TX_HIGH_ID;
};

class FeedBack {
public:
    FeedBack(uint8_t id, uint32_t range) : id(id), totalPosition(0), precision_range(range){};

    uint8_t id;

    struct {
        uint16_t position;
        int16_t speed;
        int16_t current;
        int16_t temperature;
    } RawData{};

    struct {
        float position;
        float speed;
        float current;
        float temperature;
        float lastPosition;
    } Data{};

    float totalPosition;

    uint32_t precision_range;

};


#endif //NONE_PRJ_MOTOR_H
