//
// Created by Administrator on 25-1-21.
//
#pragma once

#include "CDC/SuperCDC.hpp"
#include "CustomList/CustomList.hpp"
#include "ImageTrans/ImageTrans.hpp"
#include "RemoteControl/RemoteControl.hpp"
#include "RoboArm/RoboArm.hpp"
#include "Slope/Slope.hpp"

namespace interact_dep {
    enum class kb_state : uint8_t {
        DISABLE,
        RC_ENABLE,
        IM_ENABLE,
    };

    enum class robo_mode : uint8_t {
        NONE,
        NORMAL1,
        NORMAL2,
        XYZ,
        CUSTOM,
        VISION,
        ACTIONS,
        ACTIONS_GROUP,
        DRAW,
    };

    enum class chassis_mode : uint8_t {
        NONE,
        NORMAL,
        CLIMB,
    };
    enum class chassis_polarity : uint8_t { NONE, ANTI };

    enum class action_status : uint8_t {
        Joints,
        CartesianX,
        CartesianZ,
    };

    constexpr std::array<float, 4> default_action_speed = {480, 480, 720, 720};
    struct Actions {
        action_status status;
        bool init;
        std::array<float, 4> speed;
        union {
            struct {
                std::array<float, 6> joints;
            };
            struct {
                std::array<float, 3> pos;
                Slope axis_value;
            };
        };
        explicit Actions(action_status status = action_status::Joints, std::array<float, 4> speed=default_action_speed)
            : status(status)
            , speed(speed) {};
        explicit Actions(const std::array<float, 6>& joints, std::array<float, 4> speed=default_action_speed)
            : status(action_status::Joints)
            , speed(speed)
            , joints(joints) {};

        explicit Actions(const Slope& target, action_status status, std::array<float, 4> speed=default_action_speed)
            : status(status)
            , init(false)
            , speed(speed)
            , pos()
            , axis_value(target) {};
    };

    struct ActionsGroup {
        using event = bool (*)();
        using exe = void (*)();

        Actions* actions_list;
        uint32_t* time_list;
        event* event_list;
        exe* exe_list;
        uint8_t len;
        uint8_t index;
        uint32_t time_cnt;

        Actions* get() { return &actions_list[index]; }

        void reset() {
            actions_list->init = false;
            time_cnt           = 0;
            index              = 0;
        }

        void update() {
            if (time_cnt==0) {
                if (exe_list != nullptr && exe_list[0] != nullptr) {
                    exe_list[0]();
                }
            }
            ++time_cnt;
            auto event_bool = false;
            if (event_list != nullptr && event_list[index] != nullptr) {
                event_bool =event_list[index]();
            }
            if (time_cnt > time_list[index] || event_bool) {
                if (index < len - 1) {
                    ++index;
                    if (exe_list != nullptr && exe_list[index] != nullptr) {
                        exe_list[index]();
                    }
                    time_cnt = 1;

                    actions_list[index].init = false;
                } else {
                    if (exe_list != nullptr && exe_list[len] != nullptr) {
                        exe_list[len]();
                    }
                }
            }
        }
    };

} // namespace interact_dep

extern Interact interact;
