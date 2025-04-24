#include "ui.hpp"

void UI::delete_layer(ui_dep::operate_delete_layer delete_layer, uint8_t layer_id) {
    interaction_layer_delete_t layer_delete {.delete_type = static_cast<uint8_t>(delete_layer), .layer = layer_id};
    lock();
    type = UI::types::DELETE;
    memcpy(frame.data_frame.user_data, &layer_delete, sizeof(interaction_layer_delete_t));
    num  = 0;
    len  = sizeof(interaction_layer_delete_t);
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
        case types::NONE: break;
        default: break;
    }
}
