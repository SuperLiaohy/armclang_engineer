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
    enum class chassis_polarity : uint8_t {
        NONE,
        ANTI
    };



    struct Action {
        float* data {};
        explicit Action(uint8_t num) {
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

