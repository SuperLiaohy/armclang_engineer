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
        LOST,
        DISABLE,
        RC_ENABLE,
        IM_ENABLE,
    };

    enum class robo_mode : uint8_t {
        NONE,
        NORMAL,
        XYZ,
        RESET,
        CUSTOM,
        VISION,
        ACTIONS,
    };

    enum class chassis_mode : uint8_t {
        NONE,
        NORMAL,
        ALL,
        CUSTOM,
        VISION,
        ACTIONS,
    };

    struct receive_data_t {
        uint8_t head;
        link_receive_t joint1;
        link_receive_t joint2;
        link_receive_t joint3;
        link_receive_t joint4;
        link_receive_t joint5;
        link_receive_t joint6;
        uint8_t tail;
    } __attribute__((packed));

    struct transmit_data_t {
        uint8_t head;
        uint16_t cmd;
        link_transmit_t joint1;
        link_transmit_t joint2;
        link_transmit_t joint3;
        link_transmit_t joint4;
        link_transmit_t joint5;
        link_transmit_t joint6;
        uint8_t tail;
    } __attribute__((packed));

    struct Action {
        float* data {};
        Slope slope;
        explicit Action(uint8_t num, float step = 3, float dead_zone = 0.1)
            : slope(step, dead_zone) {
            data = reinterpret_cast<float *>(pvPortMalloc(num * sizeof(float )));
        };
    };

    struct Actions {
        uint8_t num;
        Action joint1;
        Action joint2;
        Action joint3;
        Action joint4;
        Action joint5;
        Action joint6;
        uint8_t now;
        explicit Actions(uint8_t num = 0)
            : num(num)
            , joint1(num)
            , joint2(num)
            , joint3(num)
            , joint4(num)
            , joint5(num)
            , joint6(num)
            , now(0) {};
    };

} // namespace interact_dep

extern Interact interact;
extern interact_dep::Actions test_actions;
extern interact_dep::Actions stretch;
extern interact_dep::Actions turn_up;


