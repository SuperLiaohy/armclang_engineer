//
// Created by liaohy on 24-11-25.
//
#pragma once

#include "FeedBack.hpp"
// #include "M3508.hpp"

#include <concepts>

namespace motor_const {
    constexpr float M2006Gain  = 36;
    constexpr float GM6020Gain = 1;
    constexpr float M3508Gain  = 3591.f / 187;
} // namespace motor_const

template<typename T>
concept motor_param = requires(T t, uint8_t* data) {
    typename T::Feedback;

    t.feedback.raw_data.speed;
    t.feedback.raw_data.position;
    t.feedback.raw_data.current;

    t.feedback.data.speed;
    t.feedback.data.position;
    t.feedback.data.current;

    t.detect;
    t.rx_id;
    t.reduction_ratio;
    t.get_feedback(data);
};

template<typename T>
concept motor_control = requires(T t, uint8_t* data) {
    requires requires { t.set_position(0.f); } || requires { t.set_speed(0.f); } || requires { t.set_current(0.f); };

};

struct DefaultFeedback {
    struct {
        int16_t speed;
        int16_t position;
        int16_t current;
        int16_t temperature;
    } raw_data;

    struct {
        float speed;
        float position;
        float current;
        float temperature;
        float last_position;
    } data;

    float precision_range;
    float total_position;
};

class default_motor {
public:
    using Feedback = DefaultFeedback;

public:
    Feedback feedback;
    uint16_t rx_id;
    Detect detect;
    float reduction_ratio;

    default_motor(const uint16_t rx_id, const float reduction_ratio) : feedback()
        , rx_id(rx_id)
        , detect(1000)
        , reduction_ratio(reduction_ratio) {}

    void init(uint16_t id, float reduction_ratio) {
        this->rx_id = id;
        this->reduction_ratio = reduction_ratio;
    };
    inline void get_feedback(uint8_t* data);
};

inline void default_motor::get_feedback(uint8_t* data) {
    feedback.raw_data.position    = static_cast<int16_t>(((data[0] << 8) | data[1]));
    feedback.raw_data.speed       = static_cast<int16_t>((data[2] << 8) | data[3]);
    feedback.raw_data.current     = static_cast<int16_t>((data[4] << 8) | data[5]);
    feedback.raw_data.temperature = (data[6]);

    feedback.data.position    = static_cast<float>(feedback.raw_data.position) * 360.0f / feedback.precision_range;
    feedback.data.speed       = feedback.raw_data.speed;
    feedback.data.current     = feedback.raw_data.current;
    feedback.data.temperature = feedback.raw_data.temperature;

    float dPos = feedback.data.position - feedback.data.last_position;
    if (dPos > 180) {
        dPos = dPos - 360;
    } else if (dPos < -180) {
        dPos = dPos + 360;
    }

    feedback.total_position     += (dPos / reduction_ratio);
    feedback.data.last_position  = feedback.data.position;

    detect.update();
};




// struct motor_cfg {
//     uint16_t id;
//     uint32_t range;
//     uint32_t maxInterval;
//
//     constexpr motor_cfg(uint16_t id, uint32_t range, uint32_t maxInterval = 1000) : id(id), range(range),
//         maxInterval(maxInterval) {
//     };
// };
//
// class dmMotor;
// class lkMotor;
// class M2006;
// class lkMotorBoard;

// template<typename MOTOR>
// class Motor {
// public:
//     static const FOC foc;
//
//     Motor(uint16_t id, int64_t range, uint32_t maxInterval = 1000) : motor(id), feed_back(id, range), detect(maxInterval) {};
//
//     explicit Motor(const motor_cfg &cfg) : motor(cfg.id), feed_back(cfg.id, cfg.range), detect(cfg.maxInterval) {};
//
//     MOTOR motor;
//
//     FeedBack feed_back;
//
//     Detect detect;
//
//     void get_feedback(uint8_t *data);
//     void get_feedback(uint8_t *data) requires std::same_as<MOTOR, dmMotor>;
//     void get_feedback(uint8_t *data) requires std::same_as<MOTOR, lkMotor>;
//     void get_feedback(uint8_t *data) requires std::same_as<MOTOR, lkMotorBoard>;
//
// };
//
//
// template <typename MOTOR>
// void Motor<MOTOR>::get_feedback(uint8_t *data)  {
//     feed_back.data.lastPosition = feed_back.data.position;
//     feed_back.raw_data.position = static_cast<int16_t>(((data[0] << 8) | data[1]));
//     feed_back.raw_data.speed = static_cast<int16_t>((data[2] << 8) | data[3]);
//     feed_back.raw_data.current = static_cast<int16_t>((data[4] << 8) | data[5]);
//     feed_back.raw_data.temperature = (data[6]);
//
//     feed_back.data.position = static_cast<float>(feed_back.raw_data.position) * 360.0f / feed_back.precision_range;
//     feed_back.data.speed = feed_back.raw_data.speed;
//     feed_back.data.current = feed_back.raw_data.current;
//     feed_back.data.temperature = feed_back.raw_data.temperature;
//
//     double dPos = feed_back.data.position - feed_back.data.lastPosition;
//     if (dPos > 180 ) {
//         dPos = dPos - 360;
//     }
//     else if (dPos < -180 ) {
//         dPos = dPos + 360;
//     }
//     feed_back.totalPosition += (dPos / motor.gain);
//
//     detect.update();
// }
