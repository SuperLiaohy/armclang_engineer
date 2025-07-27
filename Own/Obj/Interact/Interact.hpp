//
// Created by liaohy on 24-12-11.
//
#pragma once

#include "Interact/Interact_dep.hpp"

class Chassis;
class RoboArm;

class Interact {
public:
    Interact(const uint8_t head,const uint8_t tail, UART_HandleTypeDef* uart_rc, UART_HandleTypeDef* uart_im,
             UART_HandleTypeDef* uart_sub_board)
        : remote_control(uart_rc)
        , image_trans(uart_im)
        , pc(head, tail)
        , kb {interact_dep::kb_state::RC_ENABLE}
        , robo_arm {interact_dep::robo_mode::NONE, interact_dep::robo_mode::NONE}
        , joint_slope({Slope(0.15, 0), Slope(0.6, 0), Slope(0.3, 0), Slope(0.3, 0)})
        , chassis {interact_dep::chassis_mode::NONE, interact_dep::chassis_mode::NONE}
        , sub_board(uart_sub_board) {};

    RemoteControl remote_control;
    ImageTrans image_trans;
    SubBoard sub_board;
    PC pc;

    interact_dep::kb_state kb;
    KeyBoard key_board {};
    Key keyList[18];

    struct {
        interact_dep::robo_mode mode;
        interact_dep::robo_mode last_mode;
    } robo_arm;
    std::array<float, 6> joint {};
    std::array<Slope, 4> joint_slope;
    interact_dep::Actions* actions {};
    interact_dep::ActionsGroup* actions_group {};

    struct {
        interact_dep::chassis_mode mode;
        interact_dep::chassis_mode last_mode;
    } chassis;

    void update_chassis(Chassis& cha);
    void update_roboArm(RoboArm& Arm);

    void receive_cdc(uint8_t* data);
    void receive_rc();
    void receive_xyz(RoboArm& Arm);
    void receive_kb();
    void receive_custom(uint8_t* data);
    void receive_actions(RoboArm& Arm, float pitch);
    void receive_actions_group();

    void set_action(interact_dep::Actions& action);
    void set_action_group(interact_dep::ActionsGroup& actions_group);

    void transmit_relative_pos(const std::array<float, 6>& pos);
};

extern Interact interact;

