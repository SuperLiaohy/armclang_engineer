#include "referee_system.h"
using namespace crc;


/**
 * @brief 裁判系统数据处理
 * @note
 *
 * @param
 * @return
 */
void Class_Referee_System::Referee_data_processing(uint8_t data[], uint16_t length)
{
    auto data_origin = data;

    while((data - data_origin) < 500)//截取500个字节
    {
        if(*data == 0xA5)//寻找帧头
        {
            memcpy(&Frame_header, data, 5);
            memcpy(&cmd_id, data + 5, 2);
            crc8_check = verify_crc8_check_sum(data, 5);
            crc16_check = verify_crc16_check_sum(data, Frame_header.data_length + 9);
            data += 7;
            if((Frame_header.sof == 0xA5) && (crc8_check == 1) && (crc16_check == 1))
            {
                switch (cmd_id)
                {
                    case 0x0001://比赛状态数据
                        memcpy(&game_status, data, Frame_header.data_length);
                        break;
                    case 0x0002://场地事件数据
                        memcpy(&game_result, data, Frame_header.data_length);
                        break;
                    case 0x0003://机器人血量数据
                        memcpy(&game_robot_HP, data, Frame_header.data_length);
                        break;
                    case 0x0101://场地事件数据
                        memcpy(&event_data, data, Frame_header.data_length);
                        break;
                    case 0x0102://补给站动作标识数据
                        memcpy(&ext_supply_projectile_action, data, Frame_header.data_length);
                        break;
                    case 0x0104://裁判警告数据
                        memcpy(&referee_warning, data, Frame_header.data_length);
                        break;
                    case 0x0105://飞镖发射相关数据
                        memcpy(&dart_info, data, Frame_header.data_length);
                        break;
                    case 0x0201://机器人性能体系数据
                        memcpy(&robot_status, data, Frame_header.data_length);
                        break;
                    case 0x0202://实时功率热量数据
                        memcpy(&power_heat_data, data, Frame_header.data_length);
                        break;
                    case 0x0203://机器人位置数据
                        memcpy(&robot_pos, data, Frame_header.data_length);
                        break;
                    case 0x0204://机器人增益数据
                        memcpy(&buff, data, Frame_header.data_length);
                        break;
                    case 0x0205://空中支援时间数据
                        memcpy(&air_support_data, data, Frame_header.data_length);
                        break;
                    case 0x0206://伤害状态数据
                        memcpy(&hurt_data, data, Frame_header.data_length);
                        break;
                    case 0x0207://实时射击信息数据
                        memcpy(&shoot_data, data, Frame_header.data_length);
                        break;
                    case 0x0208://允许发弹量
                        memcpy(&projectile_allowance, data, Frame_header.data_length);
                        break;
                    case 0x0209://机器人 RFID 状态
                        memcpy(&rfid_status, data, Frame_header.data_length);
                        break;
                    case 0x020A://飞镖选手端指令数据
                        memcpy(&dart_client_cmd, data, Frame_header.data_length);
                        break;
                    case 0x020B://地面机器人位置数据
                        memcpy(&ground_robot_position, data, Frame_header.data_length);
                        break;
                    case 0x020C://雷达标记进度数据
                        memcpy(&radar_mark_data, data, Frame_header.data_length);
                        break;
                    case 0x020D://哨兵自主决策相关信息同步
                        memcpy(&sentry_info, data, Frame_header.data_length);
                        break;
                    case 0x020E://雷达自主决策信息同步
                        memcpy(&radar_info, data, Frame_header.data_length);
                        break;
                    case 0x0301://机器人交互数据
                        memcpy(&robot_interaction_data, data, Frame_header.data_length);
                        break;
                    default:
                        break;
                }
            }
            data += Frame_header.data_length + 2;
        }
        else data++;
    }
}

/**
 * @brief 数据发送
 * @note 不支持机器人间通信
 *
 * @param
 * @return
 */
uint8_t data_seq;//数据包序号
uint8_t data_send_buf[130];//发送数据缓存
int16_t Referee_data_transmit(uint16_t Child_content_ID,uint16_t Cilent_ID,uint16_t Robot_ID,uint8_t user_tx_data[])
{
    uint16_t CMD_ID = 0x0301;
    uint8_t *frame_point;//读写指针
    uint16_t frametail = 0xFFFF;
    uint16_t data_length;
    robot_interaction_data_t robot_interaction_data_send;//交互数据接收信息

    switch (Child_content_ID)
    {
        case 0x0100://选手端删除图层
            data_length = 2+6;
            break;
        case 0x0101://选手端绘制一个图形
            data_length = 15+6;
            break;
        case 0x0102://选手端绘制两个图形
            data_length = 30+6;
            break;
        case 0x0103://选手端绘制五个图形
            data_length = 75+6;
            break;
        case 0x0104://选手端绘制七个图形
            data_length = 105+6;
            break;
        case 0x0110://选手端绘制字符图形
            data_length = 45+6;
            break;
        case 0x0120://哨兵自主决策指令
            data_length = 4+6;
            break;
        case 0x0121://雷达自主决策指令
            data_length = 1+6;
            break;

        default:
            return -1;
    }
    /* 完成帧头打包 */
    frame_header Frame_head_send;//帧头
    frame_point = (uint8_t *)&Frame_head_send;
    Frame_head_send.sof = 0xA5;
    Frame_head_send.data_length = data_length;
    Frame_head_send.seq = data_seq++;
    Frame_head_send.crc8 = get_crc8_check_sum(frame_point, 4, 0xFF);

    memcpy(data_send_buf,&Frame_head_send,5);

    /*
    memcpy(data_send_buf,&Frame_head_send.SOF,1);
    memcpy(data_send_buf+1,&Frame_head_send.data_length,2);
    memcpy(data_send_buf+3,&Frame_head_send.seq,1);
    memcpy(data_send_buf+4,&Frame_head_send.CRC8,1);

    */

    memcpy(data_send_buf+5,&CMD_ID,2);

    robot_interaction_data_send.data_cmd_id = Child_content_ID;
    robot_interaction_data_send.receiver_id = Cilent_ID;
    robot_interaction_data_send.sender_id = Robot_ID;

    memcpy(data_send_buf+7,&robot_interaction_data_send,6);
    memcpy(data_send_buf+13,user_tx_data,data_length-6);

    /* 整包校验 */
    frametail = get_crc16_check_sum(data_send_buf,Frame_head_send.data_length+7,0xFFFF);
    memcpy(data_send_buf+Frame_head_send.data_length+7,&frametail,2);

    /* 数据发送 */
//    HAL_UART_Transmit_IT(&huart6,data_send_buf,Frame_head_send.data_length+9);
    return 0;
}
