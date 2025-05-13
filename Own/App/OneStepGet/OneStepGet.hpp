//
// Created by Administrator on 25-5-1.
//

#pragma once

#include "Motor/Motor.hpp"
#include "Slope/Slope.hpp"

enum class OneStepGetControl {
    AUTO,
    MANUAL,
};
enum class OneStepGetAUTO {
    NONE,
    // RESET,
    // GOLD,
};

enum class OneStepGetYStatus {
    NONE,
    UP,
    DOWN,
};

enum class OneStepGetXStatus {
    NONE,
    FRONT,
    BACK,
};




class OSG {
public:
    static OneStepGetControl mode;
    static OneStepGetAUTO auto_mode;
    OSG(const Pid& left_x_pos_pid, const Pid& left_x_speed_pid, const uint8_t left_x_id,
        const Slope& left_x_slope_cfg, const Pid& left_y_pos_pid, const Pid& left_y_speed_pid,
        const uint8_t left_y_id, const Slope& left_y_slope_cfg, const Pid& right_x_pos_pid,
        const Pid& right_x_speed_pid, const uint8_t right_x_id, const Slope& right_x_slope_cfg,
        const Pid& right_y_pos_pid, const Pid& right_y_speed_pid, const uint8_t right_y_id,
        const Slope& right_y_slope_cfg)
        : left(left_x_pos_pid, left_x_speed_pid, left_x_id, left_x_slope_cfg,
            left_y_pos_pid, left_y_speed_pid, left_y_id, left_y_slope_cfg)
        , right(right_x_pos_pid, right_x_speed_pid, right_x_id, right_x_slope_cfg,
            right_y_pos_pid, right_y_speed_pid, right_y_id, right_y_slope_cfg) {};
    class group {
    public:
        group(const Pid& x_pos_pid, const Pid& x_speed_pid, const uint8_t x_id, const Slope& x_slope_cfg,
              const Pid& y_pos_pid, const Pid& y_speed_pid, const uint8_t y_id, const Slope& y_slope_cfg)
            : X(x_pos_pid, x_speed_pid, x_id, x_slope_cfg)
            , Y(y_pos_pid, y_speed_pid, y_id, y_slope_cfg) {}
        class XGet {
        public:
            XGet(const Pid& x_pos_pid, const Pid& x_speed_pid, const uint8_t x_id, const Slope& x_slope_cfg)
                : pos(x_slope_cfg)
                , Motor(x_pos_pid, x_speed_pid, x_id) {};
            bool move_it();
            // bool move_front();
            OneStepGetXStatus status;
            bool is_block;
            Slope pos;
            int32_t block_count;
            Motor<M2006Pos> Motor;
        } X;
        class YGet {
        public:
            YGet(const Pid& y_pos_pid, const Pid& y_speed_pid, const uint8_t y_id, const Slope& y_slope_cfg)
                : pos(y_slope_cfg)
                , Motor(y_pos_pid, y_speed_pid, y_id) {};
            // bool move_up();
            bool move_it();
            OneStepGetYStatus status;
            bool is_block;
            Slope pos;
            int32_t block_count;
            Motor<M3508Pos> Motor;
        } Y;
    };

    void reset() {
        left.X.status = OneStepGetXStatus::BACK;
        left.Y.status = OneStepGetYStatus::DOWN;
        right.X.status = OneStepGetXStatus::BACK;
        right.Y.status = OneStepGetYStatus::DOWN;
        left.X.pos.target_set(-2000);
        left.Y.pos.target_set(-1300);
        right.X.pos.target_set(2300);
        right.Y.pos.target_set(1450);
    }

    group left;
    group right;
};

// extern OneStepGet one_step_get_left;
// extern OneStepGet one_step_get_right;
// extern OneStepGetControl one_step_get_control;
// extern OneStepGetAUTO one_step_get_auto;
extern OSG one_step_gets;

