//
// Created by liaohy on 24-12-11.
//

#ifndef INTERACT_H
#define INTERACT_H

#include "Interact/interact_dep.h"
//设定交互方式的统一标准为2个字节16bit的形式
class Interact {

public:
    Interact(const uint8_t& head,const uint8_t& tail, UART_HandleTypeDef* uart_rc, UART_HandleTypeDef *uart_im)
        : interaction(interact_dep::NONE)
        , last_interaction(interact_dep::NONE)
        , remote_control(uart_rc)
        , head(head)
        , tail(tail)
        , image_trans(uart_im) {
        transmit_data.head = head;
        transmit_data.tail = tail;
        interaction = interact_dep::INTERACTION::REMOTE_CTRL;
    };

    interact_dep::receive_data_t receive_data {};
    int64_t totalRoll = 0;
    float pos[3] = {85.912, 0, 241.753};

    interact_dep::transmit_data_t transmit_data {};

    RemoteControl remote_control;

    ImageTrans image_trans;

    class PC{
    public:
        SuperCDC cdc;
        interact_dep::receive_data_t receive_data {};
        interact_dep::transmit_data_t transmit_data {};
    } pc;

    interact_dep::path path;
    interact_dep::key_board key_board;
    interact_dep::robo_mode robo_mode;
    interact_dep::chassis_mode chassis_mode;

    interact_dep::INTERACTION interaction;
    interact_dep::INTERACTION last_interaction;

    void receive_cdc(uint8_t* data);

    void receive_rc();

    void receive_xyz(RoboArm& Arm);

    void receive_reset(RoboArm& Arm);

    void receive_custom(uint8_t* data);

    void transmit_relative_pos(RoboArm& Arm);
private:

    inline void transmit();

    uint8_t head;
    uint8_t tail;
};

inline void Interact::transmit()  {
    CDC_Transmit_HS(reinterpret_cast<uint8_t*>(&transmit_data), sizeof(transmit_data));
}



#endif //INTERACT_H
