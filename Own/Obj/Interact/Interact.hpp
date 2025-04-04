//
// Created by liaohy on 24-12-11.
//
#pragma once

#include "Chassis/Chassis.hpp"
#include "Interact/Interact_dep.hpp"
#include "PC/PC.hpp"
#include "SubBoard/SubBoard.hpp"
//设定交互方式的统一标准为2个字节16bit的形式
class Interact {
public:
    Interact(const uint8_t& head, const uint8_t& tail, UART_HandleTypeDef* uart_rc, UART_HandleTypeDef* uart_im, UART_HandleTypeDef* uart_sub_board)
        : kb {interact_dep::kb_state::RC_ENABLE}
        , robo_arm {interact_dep::robo_mode::NORMAL, interact_dep::robo_mode::NORMAL}
        , chassis {interact_dep::chassis_mode::ALL, interact_dep::chassis_mode::ALL}
        , remote_control(uart_rc)
        , head(head)
        , tail(tail)
        , image_trans(uart_im)
        , sub_board(uart_sub_board) {
//        transmit_data.head = head;
//        transmit_data.tail = tail;
    };

    std::array<float, 6> joint;

//    interact_dep::receive_data_t receive_data {};
    // int64_t totalRoll = 0;
//    float pos[3]      = {85.912, 0, 241.753};

//    interact_dep::transmit_data_t transmit_data {};

    RemoteControl remote_control;

    ImageTrans image_trans;

    PC pc;

    SubBoard sub_board;

    interact_dep::Actions* actions;

    Key keyList[18];
    KeyBoard key_board;

    interact_dep::path path;
    interact_dep::kb_state kb;

    struct {
        interact_dep::robo_mode mode;
        interact_dep::robo_mode last_mode;
    } robo_arm;

    struct {
        interact_dep::chassis_mode mode;
        interact_dep::chassis_mode last_mode;
    } chassis;

    void update_chassis(Chassis& cha);

    void update_roboArm(RoboArm& Arm);

    void receive_cdc(uint8_t* data);

    void receive_rc();

    void receive_xyz(RoboArm& Arm);

    void receive_reset();

    void receive_custom(uint8_t* data);

    void receive_actions(bool is_next);

    void transmit_relative_pos(const std::array<float, 6>& pos);

private:
    uint8_t head;
    uint8_t tail;
};


