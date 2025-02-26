//
// Created by lhy on 2024/9/9.
//
#pragma once


#define USING_CHASSIS 1
#if USING_CHASSIS == 1

    #include "chassis_dep.h"

class Chassis {
public:
    Chassis(SuperCan* can, const std::array<slope_cfg, 4>& move_cfg, const chassis_dep::rotate_cfg& rot, const std::array<motor_cfg, 4>& base_cfg,
            const std::array<motor_cfg, 2>& extend_cfg)
        : can(can)
        , base(base_cfg)
        , extend(extend_cfg)
        , move(move_cfg)
        , key()
        , rotate(rot) {}


    SuperCan* can;
    void send_foc();

    void send_foc(int16_t left_front, int16_t right_front, int16_t left_rear, int16_t right_rear, int16_t left, int16_t right);

    void send_base_foc(int16_t left_front, int16_t right_front, int16_t left_rear, int16_t right_rear);
    void send_base_foc();

    void send_extend_foc(int16_t left, int16_t right);

    void update_state(RemoteControl &remote_control, float relative_angle);

    void UpdateMotor();

    void UpdatePid();

    void update_slope();

    chassis_dep::base_motor base;
    chassis_dep::extend_motor extend;
private:

    chassis_dep::move move;

    chassis_dep::key key;

    chassis_dep::rotate rotate;

    void load_speed();

    friend class Interact;
    friend void chassis_w_callback(KeyEventType event);
    friend void chassis_a_callback(KeyEventType event);
    friend void chassis_s_callback(KeyEventType event);
    friend void chassis_d_callback(KeyEventType event);
    friend void chassis_q_callback(KeyEventType event);

};


extern Chassis chassis;

#endif

