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
    Interact(const uint8_t& head, const uint8_t& tail, UART_HandleTypeDef* uart_rc, UART_HandleTypeDef* uart_im,
             UART_HandleTypeDef* uart_sub_board)
        : kb {interact_dep::kb_state::RC_ENABLE}
        , robo_arm {interact_dep::robo_mode::NONE, interact_dep::robo_mode::NONE}
        , chassis {interact_dep::chassis_mode::NONE, interact_dep::chassis_mode::NONE}
        , polarity(interact_dep::chassis_polarity::NONE)
        , remote_control(uart_rc)
        , image_trans(uart_im)
        , pc(head, tail)
        , sub_board(uart_sub_board)
        , joint_slope({Slope(0.15, 0.2), Slope(0.6, 0.2), Slope(0.3, 0.2)}) {};

    RemoteControl remote_control;

    ImageTrans image_trans;

    PC pc;

    SubBoard sub_board;

    Key keyList[18];
    KeyBoard key_board {};

    interact_dep::kb_state kb;

    struct {
        interact_dep::robo_mode mode;
        interact_dep::robo_mode last_mode;
    } robo_arm;
    std::array<float, 6> joint {};
    std::array<Slope, 3> joint_slope;
    interact_dep::Actions* actions {};
    interact_dep::ActionsGroup* actions_group {};

    struct {
        interact_dep::chassis_mode mode;
        interact_dep::chassis_mode last_mode;
    } chassis;

    interact_dep::chassis_polarity polarity;

    void update_chassis(Chassis& cha);

    void update_roboArm(RoboArm& Arm);

    void receive_cdc(uint8_t* data);

    void receive_rc();

    void receive_xyz(RoboArm& Arm);

    void receive_custom(uint8_t* data);

    void receive_actions(RoboArm& Arm, float pitch);

    void receive_actions_group();

    void transmit_relative_pos(const std::array<float, 6>& pos);
};
