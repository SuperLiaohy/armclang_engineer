//
// Created by liaohy on 24-12-11.
//

#ifndef INTERACT_H
#define INTERACT_H

#include "CDC/SuperCDC.h"
#include "CustomList/CustomList.h"
#include "RemoteControl/RemoteControl.h"
#include "RoboArm/RoboArm.h"
//设定交互方式的统一标准为2个字节16bit的形式
class RoboArm;

class Interact {
public:
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



public:
    Interact(uint8_t&& head, uint8_t&& tail, RemoteControl* remoteControl)
        : interaction(REMOTE_CTRL)
        , last_interaction(REMOTE_CTRL)
        , remoteControl(remoteControl)
        , head(head)
        , tail(tail) {
        transmit_data.head = head;
        transmit_data.tail = tail;
        interaction = INTERACTION::REMOTE_CTRL;
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
    } __attribute__((packed)) receive_data {};

    int64_t totalRoll = 0;

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
    } __attribute__((packed)) transmit_data {};

    float pos[3] = {-46.475, 0, 252.320};

    INTERACTION interaction;
    INTERACTION last_interaction;

    void receive_cdc(uint8_t* data);

    void receive_rc(RoboArm& Arm);

    void receive_xyz(RoboArm& Arm);

    void receive_reset(RoboArm& Arm);

    void receive_custom(uint8_t* data);

    void transmit_relative_pos(RoboArm& Arm);

private:
    inline void transmit();

    RemoteControl* remoteControl;

    uint8_t head;
    uint8_t tail;
};

inline void Interact::transmit()  {
    CDC_Transmit_HS(reinterpret_cast<uint8_t*>(&transmit_data), sizeof(transmit_data));
}

extern Interact interact;

#endif //INTERACT_H
