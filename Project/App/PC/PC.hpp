//
// Created by liaohy on 25-3-18.
//

#pragma once

#include "CDC/SuperCDC.hpp"

class PC : public SuperCDC {
#pragma pack(push, 1)
    struct rx_joint_cfg {
        int16_t angle;
    };
#pragma pack(pop)
#pragma pack(push, 1)
    struct tx_joint_cfg {
        int16_t angle;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct user_rx_data {
        rx_joint_cfg joint1;
        rx_joint_cfg joint2;
        rx_joint_cfg joint3;
        rx_joint_cfg joint4;
        rx_joint_cfg joint5;
        rx_joint_cfg joint6;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct rx_frame {
        uint8_t head;
        user_rx_data data;
        uint8_t tail;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct user_tx_data {
        tx_joint_cfg joint1;
        tx_joint_cfg joint2;
        tx_joint_cfg joint3;
        tx_joint_cfg joint4;
        tx_joint_cfg joint5;
        tx_joint_cfg joint6;
    };
#pragma pack(pop)
#pragma pack(push, 1)
    struct tx_frame {
        uint8_t head;
        uint16_t cmd;
        user_tx_data data;
        uint8_t tail;
    };
#pragma pack(pop)
public:
    PC(const uint8_t head, const uint8_t tail)
        : head(head)
        , tail(tail) {}

    rx_frame rx_frame {};
    tx_frame tx_frame {};

    const uint8_t head;
    const uint8_t tail;
};
