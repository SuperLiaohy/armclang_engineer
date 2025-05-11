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
    typedef struct link_receive {
        int16_t angle;
        //        int16_t speed;
    } __attribute__((packed)) link_receive_t;

    typedef struct link_transmit {
        int16_t angle;
        //        int16_t speed;
    } __attribute__((packed)) link_transmit_t;

    enum class path : uint8_t {
        REMOTE_CTRL,
        IMAGE_TRANSMIT,
        PC,
    };

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
        RESET,
        CUSTOM,
        VISION,
        ACTIONS,
        DRAW,
    };

    enum class chassis_mode : uint8_t {
        NONE,
        NORMAL,
        CLIMB,
    };
    enum class chassis_polarity : uint8_t { NONE, ANTI };

    struct Action {
        float* data {};
        explicit Action(uint8_t num) { data = reinterpret_cast<float*>(pvPortMalloc(num * sizeof(float))); };
    };

    enum class action_status : uint8_t {
        Joints,
        CartesianX,
        CartesianZ,
    };

    struct Actions {
        action_status status;
        union {
            struct {
                std::array<float, 6> joints;
            };
            struct {
                bool init;
                std::array<float, 3> pos;
                Slope axis_value;
            };
        };
        explicit Actions(action_status status = action_status::Joints)
            : status(status) {};
        explicit Actions(const std::array<float, 6>& joints)
            : status(action_status::Joints)
            , joints(joints) {};

        explicit Actions(const Slope& target, action_status status, const std::array<float, 3>& pos = {})
            : status(status), axis_value(target), pos(pos),init(false) {};
    };

} // namespace interact_dep

extern Interact interact;
