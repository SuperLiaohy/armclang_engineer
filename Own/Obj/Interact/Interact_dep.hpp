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
        XYZ,
    };

    struct Actions {
        static constexpr std::array<float, 4> default_action_speed = {360, 360, 360, 360};
        static constexpr std::array<float, 3> default_pos_step = {0.2,0.2,0.2};
        const action_status status;
        uint32_t time;
        bool init;
        union {
            struct {
                std::array<float, 6> joints;
                std::array<float, 4> speed;
            };
            struct {
                std::array<float, 3> zyz;
                Slope Xaxis;
                Slope Yaxis;
                Slope Zaxis;
            };
        };
        explicit Actions(const std::array<float, 6>& joints, std::array<float, 4> speed=default_action_speed, uint32_t time = 0)
            : status(action_status::Joints)
            , speed(speed)
            , joints(joints)
            , time(time){};

        explicit Actions(const std::array<float, 3>& posi, const std::array<float, 3>& post, std::array<float, 3> step=default_pos_step, uint32_t time = 0)
            : status(action_status::XYZ)
            , init(false)
            , Xaxis(step[0],step[0],posi[0])
            , Yaxis(step[1],step[1],posi[1])
            , Zaxis(step[2],step[2],posi[2])
            , zyz(post)
            , time(time)
            {};

        void setup_step(float* data, uint32_t time) {
            if (status == action_status::XYZ) {
                if (time!=0) {
                    Xaxis.step_set(my_abs(data[0] - Xaxis.target) / time);
                    Yaxis.step_set(my_abs(data[1] - Yaxis.target) / time);
                    Zaxis.step_set(my_abs(data[2] - Zaxis.target) / time);
                }
            }
        }
        void setup_step(float* data) {
            if (status == action_status::XYZ) {
                if (time!=0) {
                    Xaxis.step_set(my_abs(data[0] - Xaxis.target) / time);
                    Yaxis.step_set(my_abs(data[1] - Yaxis.target) / time);
                    Zaxis.step_set(my_abs(data[2] - Zaxis.target) / time);
                }
            }
        }
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
