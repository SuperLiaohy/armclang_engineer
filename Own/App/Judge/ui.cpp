#include "ui.hpp"

void UI::delete_layer(ui_dep::operate_delete_layer delete_layer, uint8_t layer_id) {
    interaction_layer_delete_t layer_delete {.delete_type = static_cast<uint8_t>(delete_layer), .layer = layer_id};
    lock();
    type = UI::types::DELETE;
    memcpy(frame.data_frame.user_data, &layer_delete, sizeof(interaction_layer_delete_t));
    num  = 0;
    len  = sizeof(interaction_layer_delete_t);
    sum += len + 15; // 6 + 9
    unlock();
}

void UI::operate_str(const uint8_t* name, operation op, uint8_t layer_id, color col, uint16_t font, uint16_t str_len, uint16_t width, uint16_t start_x, uint16_t start_y,const uint8_t* str) {
    interaction_figure_t string = {
        .figure_name  = {name[0], name[1], name[2]},
        .operate_type = static_cast<uint8_t>(op),
        .figure_type  = static_cast<uint8_t>(UI::graphic::STR),
        .layer        = layer_id,
        .color        = static_cast<uint8_t>(col),
        .details_a    = font,
        .details_b    = str_len,
        .width        = width,
        .start_x      = start_x,
        .start_y      = start_y,
    };
    lock();
    if (type != UI::types::DELETE) {
        type = UI::types::STRING;
        memcpy(&frame.data_frame.user_data[0], &string, sizeof(interaction_figure_t));
        memcpy(&frame.data_frame.user_data[sizeof(interaction_figure_t)], str, str_len);
        num  = 0;
        len  = 45;
        sum += len + 15;
    }
    unlock();
}

void UI::operate_fig(uint8_t* name, operation op, graphic graph, uint8_t layer_id, color col, uint16_t detail_a, uint16_t detail_b, uint16_t width, uint16_t start_x, uint16_t start_y, uint16_t detail_c, uint16_t detail_d, uint16_t detail_e) {
    interaction_figure_t fig = {
        .figure_name  = {name[0], name[1], name[2]},
        .operate_type = static_cast<uint8_t>(op),
        .figure_type  = static_cast<uint8_t>(graph),
        .layer        = layer_id,
        .color        = static_cast<uint8_t>(col),
        .details_a    = detail_a,
        .details_b    = detail_b,
        .width        = width,
        .start_x      = start_x,
        .start_y      = start_y,
        .details_c    = detail_c,
        .details_d    = detail_d,
        .details_e    = detail_e,
    };
    lock();
    if (type == UI::types::FIGURE || type == UI::types::NONE) {
        type = UI::types::FIGURE;
        memcpy(&frame.data_frame.user_data[len], &fig, sizeof(interaction_figure_t));
        ++num;
        len += sizeof(interaction_figure_t);
        sum += len + 15;
    }
    unlock();
}
void UI::add_frame_header() {
    switch (type) {
        case UI::types::DELETE:
            frame.data_frame.data_cmd_id = 0x100;
            break;
        case UI::types::STRING:
            frame.data_frame.data_cmd_id = 0x110;
            break;
        case UI::types::FIGURE:
            switch (num) {
                case 1:
                    frame.data_frame.data_cmd_id = 0x101;
                    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(&frame), len + 15);
                    break;
                case 2:
                    frame.data_frame.data_cmd_id = 0x102;
                    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(&frame), len + 15);
                    break;
                case 3:
                    memset(&frame.data_frame.user_data[len], 0, 30);
                    len+=30;
                    frame.data_frame.data_cmd_id = 0x103;
                    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(&frame), len + 15);
                    break;
                case 4:
                    memset(&frame.data_frame.user_data[len], 0, 15);
                    len+=15;
                    frame.data_frame.data_cmd_id = 0x103;
                    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(&frame), len + 15);
                    break;
                case 5:
                    frame.data_frame.data_cmd_id = 0x103;
                    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(&frame), len + 15);
                    break;
                case 6:
                    memset(&frame.data_frame.user_data[len], 0, 15);
                    len+=15;
                    frame.data_frame.data_cmd_id = 0x104;
                    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(&frame), len + 15);
                    break;
                case 7:
                    frame.data_frame.data_cmd_id = 0x104;
                    crc::append_crc16_check_sum(reinterpret_cast<uint8_t*>(&frame), len + 15);
                    break;
            }
            break;
    }
}

#include "Judge/referee_system.h"
#include "Judge/ui.hpp"

/* Private macros ------------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t UI_reset_flag = 1; //UI刷新标志

uint8_t UI_tx_buf[300]; //UI发送缓冲
/* Private function declarations ---------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/**
 * @brief 绘制图层9的内容
 * @note 静态
 *
 * @param
 * @return
 */
interaction_figure_4_t UI_layer_9;
void UI_draw_layer_9(void) {
    //    uint8_t name1[] = "091";//图片名称，用于标识更改
    //    uint8_t name2[] = "092";
    //    uint8_t name3[] = "093";
    //    uint8_t name4[] = "094";
    //    uint16_t middle_x = 970;
    //
    //    Referee_System.Draw_line(&UI_layer_9.interaction_figure[0],name1,UI_GRAPHIC_ADD,9,UI_COLOR_WHITE,1,middle_x,240,middle_x,560);
    //    Referee_System.Draw_line(&UI_layer_9.interaction_figure[1],name2,UI_GRAPHIC_ADD,9,UI_COLOR_WHITE,1,middle_x-14,512,middle_x+14,512);
    //    Referee_System.Draw_line(&UI_layer_9.interaction_figure[2],name3,UI_GRAPHIC_ADD,9,UI_COLOR_WHITE,1,middle_x-10,500,middle_x+10,500);
    //    Referee_System.Draw_line(&UI_layer_9.interaction_figure[3],name4,UI_GRAPHIC_ADD,9,UI_COLOR_WHITE,1,middle_x-80,461,middle_x+80,461);
    //
    //    memcpy(UI_tx_buf,&UI_layer_9,15*7);
    //    Referee_data_transmit(UI_DATA_DRAW_7, Referee_System.robot_status.robot_id + 256, Referee_System.robot_status.robot_id, UI_tx_buf);
    //    memset(UI_tx_buf,0,sizeof(UI_tx_buf));
}

/**
 * @brief 绘制图层8的内容
 * @note 包括电容外框，功率限制外框，枪管指向，摩擦轮转速框，自瞄框，倒车雷达，静态
 *
 * @param
 * @return
 */
//interaction_figure_4_t UI_layer_8;
//void UI_draw_layer_8(void)
//{
//    uint8_t name1[] = "081";
//    uint8_t name2[] = "082";
//    uint8_t name3[] = "083";
//    uint8_t name4[] = "084";
//    uint8_t name5[] = "085";
//    uint8_t name6[] = "086";
//
//    Referee_System.Draw_rectangle(&UI_layer_8.interaction_figure[0],name1,UI_GRAPHIC_ADD,8,UI_COLOR_CYAN,3,50,650,250,800);//抬升框
//    Referee_System.Draw_line(&UI_layer_8.interaction_figure[1],name2,UI_GRAPHIC_ADD,8,UI_COLOR_MAIN,2,1060,600,1075,300);
//    Referee_System.Draw_line(&UI_layer_8.interaction_figure[2],name3,UI_GRAPHIC_ADD,8,UI_COLOR_MAIN,2,940,600,925,300);
//    Referee_System.Draw_line(&UI_layer_8.interaction_figure[3],name4,UI_GRAPHIC_ADD,8,UI_COLOR_GREEN,2,788,650,1098,645); //小资源岛横线
//    Referee_System.Draw_line(&UI_layer_8.interaction_figure[4],name5,UI_GRAPHIC_ADD,8,UI_COLOR_GREEN,2,814,725,788,650); //小资源岛左竖线
//    Referee_System.Draw_line(&UI_layer_8.interaction_figure[5],name6,UI_GRAPHIC_ADD,8,UI_COLOR_GREEN,2,1071,721,1098,645);   //小资源岛右竖线
//
//
//    memcpy(UI_tx_buf,&UI_layer_8,15*7);
//    Referee_data_transmit(UI_DATA_DRAW_7, Referee_System.robot_status.robot_id + 256, Referee_System.robot_status.robot_id, UI_tx_buf);
//    memset(UI_tx_buf,0,sizeof(UI_tx_buf));
//}

/**
 * @brief 绘制图层6的内容
 * @note 机械臂状态
 *
 * @param
 * @return
 */
//ext_client_custom_character_t UI_arm_state;
//void UI_draw_arm_state(void)
//{
//    uint8_t name1[] = "061";//图片名称，用于标识更改
//
//    uint8_t str_arm_state[8];
//    if (Engineer_RobotArm.Get_RobotArm_Status() == Enum_RobotArm_Status::RobotArm_Status_Die){
//        memcpy(str_arm_state, "Arm_DIE", 8);
//    }
//    else if (Engineer_RobotArm.Get_RobotArm_Status() == Enum_RobotArm_Status::RobotArm_Status_Remote){
//        memcpy(str_arm_state, "Remote", 8);
//    }
//    else{
//        if (Engineer_RobotArm.Get_FSM_State_Code() == 0x00)
//        {
//            memcpy(str_arm_state, "Custom", 8);
//        }
//        else if (Engineer_RobotArm.Get_FSM_State_Code() == 0x01)
//        {
//            memcpy(str_arm_state, "PBack", 8);
//        }
//        else if (Engineer_RobotArm.Get_FSM_State_Code() == 0x02)
//        {
//            memcpy(str_arm_state, "PFront", 8);
//        }
//        else if (Engineer_RobotArm.Get_FSM_State_Code() == 0x03)
//        {
//            memcpy(str_arm_state, "TBack", 8);
//        }
//        else if (Engineer_RobotArm.Get_FSM_State_Code() == 0x04)
//        {
//            memcpy(str_arm_state, "TFront", 8);
//        }
//        else if (Engineer_RobotArm.Get_FSM_State_Code() == 0x07)
//        {
//            memcpy(str_arm_state, "BigLand", 8);
//        }
//        else if (Engineer_RobotArm.Get_FSM_State_Code() == 0x08)
//        {
//            memcpy(str_arm_state, "defend", 8);
//        }
//        else{
//            memcpy(str_arm_state, "No_Ctrl", 8);
//        }
//    }
//
//    if(UI_reset_flag == 1)
//    {
//        Referee_System.Draw_string(&UI_arm_state,name1,UI_GRAPHIC_ADD,6,UI_COLOR_ORANGE,4,1450,800,40,str_arm_state,sizeof(str_arm_state));
//    }
//    else
//    {
//        Referee_System.Draw_string(&UI_arm_state,name1,UI_GRAPHIC_MODIFY,6,UI_COLOR_ORANGE,4,1450,800,40,str_arm_state,sizeof(str_arm_state));
//    }
//
//    memcpy(UI_tx_buf,&UI_arm_state,45);
//    Referee_data_transmit(UI_DATA_DRAW_CHAR, Referee_System.robot_status.robot_id + 256, Referee_System.robot_status.robot_id, UI_tx_buf);
//    memset(UI_tx_buf,0,sizeof(UI_tx_buf));
//}

/**
 * @brief 绘制图层7的内容
 * @note 吸盘状态
 *
 * @param
 * @return
 */
//ext_client_custom_character_t UI_sucker_state;
//void UI_draw_sucker_state(void)
//{
//    uint8_t name1[] = "071";//图片名称，用于标识更改
//
//    uint8_t str_sucker_state[8];
//    if (Engineer_RobotArm.Get_Pump_Status()==Enum_Pump_Status::Pump_Status_ON){
//        memcpy(str_sucker_state, "Pump_On", 8);
//    }
//    else{
//        memcpy(str_sucker_state, "PumpOff", 8);
//    }
//
//    if(UI_reset_flag == 1)
//    {
//        Referee_System.Draw_string(&UI_sucker_state,name1,UI_GRAPHIC_ADD,7,UI_COLOR_ORANGE,4,1450,700,40,str_sucker_state,sizeof(str_sucker_state));
//    }
//    else
//    {
//        Referee_System.Draw_string(&UI_sucker_state,name1,UI_GRAPHIC_MODIFY,7,UI_COLOR_ORANGE,4,1450,700,40,str_sucker_state,sizeof(str_sucker_state));
//    }
//
//    memcpy(UI_tx_buf,&UI_sucker_state,45);
//    Referee_data_transmit(UI_DATA_DRAW_CHAR, Referee_System.robot_status.robot_id + 256, Referee_System.robot_status.robot_id, UI_tx_buf);
//    memset(UI_tx_buf,0,sizeof(UI_tx_buf));
//}

/**
 * @brief 绘制图层6的内容
 * @note 动态，前后抬升机构的高度
 *
 * @param
 * @return
 */
//interaction_figure_4_t UI_layer_5;
//void UI_draw_layer_5(void)
//{
//    uint8_t name1[] = "051";//图片名称，用于标识更改
//    uint8_t name2[] = "052";
//
//    auto front_height = (int32_t)(Engineer_Chassis.Get_Lift_Front_Angle() * 1000.0f / Engineer_Chassis.Angle_Lift_Front_Max);
//    auto back_height = (int32_t)(-Engineer_Chassis.Get_Lift_Back_Angle() * 1000.0f / Engineer_Chassis.Angle_Lift_Back_Max);
//
//    if(UI_reset_flag == 1)
//    {
//        Referee_System.Draw_int(&UI_layer_5.interaction_figure[0],name1,UI_GRAPHIC_ADD,5,UI_COLOR_ORANGE,5,70,780,30,
//                                front_height);//前抬升
//        Referee_System.Draw_int(&UI_layer_5.interaction_figure[1],name2,UI_GRAPHIC_ADD,5,UI_COLOR_ORANGE,5,70,720,30,back_height);//后抬升
//    }
//    else
//    {
//        Referee_System.Draw_int(&UI_layer_5.interaction_figure[0],name1,UI_GRAPHIC_MODIFY,5,UI_COLOR_ORANGE,5,70,780,30,
//                                front_height);//前抬升
//        Referee_System.Draw_int(&UI_layer_5.interaction_figure[1],name2,UI_GRAPHIC_MODIFY,5,UI_COLOR_ORANGE,5,70,720,30,back_height);//后抬升
//    }
//
//    memcpy(UI_tx_buf,&UI_layer_5,15*7);
//    Referee_data_transmit(UI_DATA_DRAW_7, Referee_System.robot_status.robot_id + 256, Referee_System.robot_status.robot_id, UI_tx_buf);
//    memset(UI_tx_buf,0,sizeof(UI_tx_buf));
//
//}
/************************ COPYRIGHT(C) USTC-ROBOWALKER **************************/
