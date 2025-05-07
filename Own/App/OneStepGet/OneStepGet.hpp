//
// Created by Administrator on 25-5-1.
//

#pragma once

enum class OneStepGetControl {
    AUTO,
    MANUAL,
};
enum class OneStepGetAUTO {
    NONE,
    GOT_Y,
    GOT_X,
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

#include "Motor/M2006Diff.hpp"
#include "Motor/M3508.hpp"
#include "Motor/Motor.hpp"
class OneStepGet {
public:
    friend void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef* hfdcan, uint32_t RxFifo0ITs);
    friend void OneStepGetTask();

    OneStepGet(const Pid& XcfgSpeed, const Pid& XcfgPos, const uint16_t Xid, const Pid& YcfgSpeed, const Pid& YcfgPos,
               const uint16_t Yid)
        : XMotor(XcfgPos, XcfgSpeed, Xid)
        , YMotor(YcfgPos, YcfgSpeed, Yid)
        , x(OneStepGetXStatus::NONE)
        , y(OneStepGetYStatus::NONE)
        , x_is_block(false)
        , y_is_block(false) {};

    float move_front(float& target_pos, bool is_get, bool is_block);
    float move_back(float& target_pos, bool is_block);

    float move_upward(float& target_pos, bool is_block);
    float move_down(float& target_pos, bool is_get, bool is_block);

    float x_target;
    float y_target;

    OneStepGetXStatus x;
    bool x_is_block;
    OneStepGetYStatus y;
    bool y_is_block;

private:
    Motor<M2006Pos> XMotor;
    Motor<M3508Pos> YMotor;
};
#include "Slope/Slope.hpp"

class OSG {
public:
    static OneStepGetControl mode;
    static OneStepGetAUTO auto_mode;
    OSG(const Pid& left_x_pos_pid, const Pid& left_x_speed_pid, const uint8_t left_x_id,
        const slope_cfg& left_x_slope_cfg, const Pid& left_y_pos_pid, const Pid& left_y_speed_pid,
        const uint8_t left_y_id, const slope_cfg& left_y_slope_cfg, const Pid& right_x_pos_pid,
        const Pid& right_x_speed_pid, const uint8_t right_x_id, const slope_cfg& right_x_slope_cfg,
        const Pid& right_y_pos_pid, const Pid& right_y_speed_pid, const uint8_t right_y_id,
        const slope_cfg& right_y_slope_cfg)
        : left(left_x_pos_pid, left_x_speed_pid, left_x_id, left_x_slope_cfg,
            left_y_pos_pid, left_y_speed_pid, left_y_id, left_y_slope_cfg)
        , right(right_x_pos_pid, right_x_speed_pid, right_x_id, right_x_slope_cfg,
            right_y_pos_pid, right_y_speed_pid, right_y_id, right_y_slope_cfg) {};
    class group {
    public:
        group(const Pid& x_pos_pid, const Pid& x_speed_pid, const uint8_t x_id, const slope_cfg& x_slope_cfg,
              const Pid& y_pos_pid, const Pid& y_speed_pid, const uint8_t y_id, const slope_cfg& y_slope_cfg)
            : X(x_pos_pid, x_speed_pid, x_id, x_slope_cfg)
            , Y(y_pos_pid, y_speed_pid, y_id, y_slope_cfg) {}
        class XGet {
        public:
            XGet(const Pid& x_pos_pid, const Pid& x_speed_pid, const uint8_t x_id, const slope_cfg& x_slope_cfg)
                : pos(x_slope_cfg)
                , Motor(x_pos_pid, x_speed_pid, x_id) {};
            bool move_back();
            bool move_front(bool is_get);
            OneStepGetXStatus status;
            bool is_block;
            Slope pos;
            int32_t block_count;
            Motor<M2006Pos> Motor;
        } X;
        class YGet {
        public:
            YGet(const Pid& y_pos_pid, const Pid& y_speed_pid, const uint8_t y_id, const slope_cfg& y_slope_cfg)
                : pos(y_slope_cfg)
                , Motor(y_pos_pid, y_speed_pid, y_id) {};
            bool move_up();
            bool move_down(bool is_get);
            OneStepGetYStatus status;
            bool is_block;
            Slope pos;
            int32_t block_count;
            Motor<M3508Pos> Motor;
        } Y;
    };

    group left;
    group right;
};

// extern OneStepGet one_step_get_left;
// extern OneStepGet one_step_get_right;
// extern OneStepGetControl one_step_get_control;
// extern OneStepGetAUTO one_step_get_auto;
extern OSG one_step_gets;

