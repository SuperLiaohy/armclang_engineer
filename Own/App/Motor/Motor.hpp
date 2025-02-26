//
// Created by liaohy on 24-11-25.
//
#pragma once

#include "FeedBack.hpp"

#include <concepts>

namespace motor_const {
    constexpr float M2006Gain = 36;
    constexpr float GM6020Gain = 1;
    constexpr float M3508Gain = 3591.f/187;
}

struct motor_cfg {
    uint16_t id;
    uint32_t range;
    uint32_t maxInterval;

    constexpr motor_cfg(uint16_t id, uint32_t range, uint32_t maxInterval = 1000) : id(id), range(range), maxInterval(maxInterval) {};
};

class dmMotor;
class lkMotor;
class M2006;
class lkMotorBoard;

template<typename MOTOR>
class Motor {
public:
    static const FOC foc;

    Motor(uint16_t id, int64_t range, uint32_t maxInterval = 1000) : motor(id), feed_back(id, range), detect(maxInterval) {};

    explicit Motor(const motor_cfg &cfg) : motor(cfg.id), feed_back(cfg.id, cfg.range), detect(cfg.maxInterval) {};

    MOTOR motor;

    FeedBack feed_back;

    Detect detect;

    void readData(uint8_t *data);
    void readData(uint8_t *data) requires std::same_as<MOTOR, dmMotor>;
    void readData(uint8_t *data) requires std::same_as<MOTOR, lkMotor>;
    void readData(uint8_t *data) requires std::same_as<MOTOR, lkMotorBoard>;

};


template <typename MOTOR>
void Motor<MOTOR>::readData(uint8_t *data)  {
    feed_back.Data.lastPosition = feed_back.Data.position;
    feed_back.RawData.position = static_cast<int16_t>(((data[0] << 8) | data[1]));
    feed_back.RawData.speed = static_cast<int16_t>((data[2] << 8) | data[3]);
    feed_back.RawData.current = static_cast<int16_t>((data[4] << 8) | data[5]);
    feed_back.RawData.temperature = (data[6]);

    feed_back.Data.position = static_cast<float>(feed_back.RawData.position) * 360.0f / feed_back.precision_range;
    feed_back.Data.speed = feed_back.RawData.speed;
    feed_back.Data.current = feed_back.RawData.current;
    feed_back.Data.temperature = feed_back.RawData.temperature;

    double dPos = feed_back.Data.position - feed_back.Data.lastPosition;
    if (dPos > 180 ) {
        dPos = dPos - 360;
    }
    else if (dPos < -180 ) {
        dPos = dPos + 360;
    }
    feed_back.totalPosition += (dPos / motor.gain);

    detect.update();
}




