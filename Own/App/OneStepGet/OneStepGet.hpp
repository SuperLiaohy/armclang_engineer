//
// Created by Administrator on 25-5-1.
//

#pragma once

#include "Motor/Motor.hpp"
#include "Slope/Slope.hpp"

enum class OneStepGetControl {
    MANUAL,
    AUTO,
    ROBO_ARM,
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

class Translation {
    enum class state {
        RESET,
        P_BLOCK,
        N_BLOCK,
        MOVE,
    };

public:
    Translation(const Pid &x_pos_pid, const Pid &x_speed_pid, const uint8_t x_id, const Slope &x_slope_cfg,
                int16_t pThreshold, int16_t nThreshold)
            : axis(x_slope_cfg), pblock_count(0),nblock_count(0), pThresholdBlock(pThreshold), nThresholdBlock(nThreshold),
              Motor(x_pos_pid, x_speed_pid, x_id) {};


    void move_handle();
    void state_handle();

    void set_target(float target) { axis.target_set(target); };
    void set_step(float step) {axis.step_set(step);};

    void set_positive_threshold(int16_t threshold) { pThresholdBlock = threshold; };
    void set_negative_threshold(int16_t threshold) { nThresholdBlock = threshold; };

    void set_state(state s, float param) {
        if (s == state::RESET) {
            if (param>0)
                polarity = true;
            else
                polarity = false;
            this->s = state::RESET;
        } else if (this->s != state::P_BLOCK && this->s != state::N_BLOCK) {
            this->s = s;
            axis.target_set(param);
        }
    }

private:
    state s;
    bool polarity;

    Slope axis;
    int32_t pblock_count;
    int32_t nblock_count;
    int16_t pThresholdBlock;
    int16_t nThresholdBlock;
    Motor<M2006Pos> Motor;
};

class OSG {
public:
    static OneStepGetControl mode;
    static OneStepGetAUTO auto_mode;

    OSG(const Pid &left_x_pos_pid, const Pid &left_x_speed_pid, const uint8_t left_x_id,
        const Slope &left_x_slope_cfg, const Pid &left_y_pos_pid, const Pid &left_y_speed_pid,
        const uint8_t left_y_id, const Slope &left_y_slope_cfg, const Pid &right_x_pos_pid,
        const Pid &right_x_speed_pid, const uint8_t right_x_id, const Slope &right_x_slope_cfg,
        const Pid &right_y_pos_pid, const Pid &right_y_speed_pid, const uint8_t right_y_id,
        const Slope &right_y_slope_cfg)
            : left(left_x_pos_pid, left_x_speed_pid, left_x_id, left_x_slope_cfg,
                   left_y_pos_pid, left_y_speed_pid, left_y_id, left_y_slope_cfg),
              right(right_x_pos_pid, right_x_speed_pid, right_x_id, right_x_slope_cfg,
                    right_y_pos_pid, right_y_speed_pid, right_y_id, right_y_slope_cfg) {};

    class group {
    public:
        group(const Pid &x_pos_pid, const Pid &x_speed_pid, const uint8_t x_id, const Slope &x_slope_cfg,
              const Pid &y_pos_pid, const Pid &y_speed_pid, const uint8_t y_id, const Slope &y_slope_cfg)
                : X(x_pos_pid, x_speed_pid, x_id, x_slope_cfg), Y(y_pos_pid, y_speed_pid, y_id, y_slope_cfg) {}

        class XGet {
        public:
            XGet(const Pid &x_pos_pid, const Pid &x_speed_pid, const uint8_t x_id, const Slope &x_slope_cfg)
                    : pos(x_slope_cfg), Motor(x_pos_pid, x_speed_pid, x_id) {};

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
            YGet(const Pid &y_pos_pid, const Pid &y_speed_pid, const uint8_t y_id, const Slope &y_slope_cfg)
                    : pos(y_slope_cfg), Motor(y_pos_pid, y_speed_pid, y_id) {};

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

