//
// Created by lhy on 2024/9/9.
//
#pragma once


#include "CAN/SuperCan.hpp"
#include "Pid/DoublePid.hpp"
#include "Detect/Detect.hpp"

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
    } raw_data{};

    struct {
        float position;
        float speed;
        float current;
        float temperature;
        float lastPosition;
    } data{};

    float totalPosition;

    uint32_t precision_range;

};


