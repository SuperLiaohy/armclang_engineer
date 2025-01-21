//
// Created by Administrator on 25-1-21.
//

#ifndef ARMCLANG_ENGINEER_INTERACT_DEP_H
#define ARMCLANG_ENGINEER_INTERACT_DEP_H
#include "CDC/SuperCDC.h"
#include "CustomList/CustomList.h"
#include "RemoteControl/RemoteControl.h"
#include "RoboArm/RoboArm.h"
#include "ImageTrans/ImageTrans.h"

namespace interact_dep {
    typedef struct link_receive {
        int16_t angle;
//        int16_t speed;
    } __attribute__((packed)) link_receive_t;

    typedef struct link_transmit {
        int16_t angle;
//        int16_t speed;
    } __attribute__((packed)) link_transmit_t;



    enum INTERACTION {
        NONE,
        CUSTOM,
        VISION,
        REMOTE_CTRL,
        REMOTE_CTRL_XYZ,
        IMAGE_TRANSMIT,
        REMOTE_CTRL_RESET,
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
}


extern Interact interact;

#endif //ARMCLANG_ENGINEER_INTERACT_DEP_H
