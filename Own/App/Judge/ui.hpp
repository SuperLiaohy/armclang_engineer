#pragma once

#include "referee_system.h"

namespace ui_dep {
    struct basic_graphic {
        uint8_t figure_name[3];
        uint32_t operate_type : 3;
        uint32_t figure_type : 3;
        uint32_t layer : 4;
        uint32_t color : 4;
        uint32_t details_a : 9;
        uint32_t details_b : 9;
        uint32_t width : 10;
        uint32_t start_x : 11;
        uint32_t start_y : 11;
        uint32_t details_c : 10;
        uint32_t details_d : 11;
        uint32_t details_e : 11;
    } __attribute__((packed));

    template<typename T>
    concept ui_item = requires(T t) { t.set(std::declval<basic_graphic*>()); };

    enum class types : uint8_t {
        DELETE,
        FIGURE,
        STRING,
        NONE,
    };

    enum class operate_delete_layer : uint8_t {
        NONE  = 0,
        LAYER = 1,
        ALL   = 2,
    };

    enum class operation : uint8_t {
        NONE   = 0,
        ADD    = 1,
        MODIFY = 2,
        DELETE = 3,
    };

    enum class graphic : uint8_t {
        LINE    = 0,
        RECT    = 1,
        CIRCLE  = 2,
        ELLIPSE = 3,
        ARC     = 4,
        FLOAT   = 5,
        INT     = 6,
        STR     = 7,
    };

    enum class layer {
        LAYER_0 = 0,
        LAYER_1 = 1,
        LAYER_2 = 2,
        LAYER_3 = 3,
        LAYER_4 = 4,
        LAYER_5 = 5,
        LAYER_6 = 6,
        LAYER_7 = 7,
        LAYER_8 = 8,
        LAYER_9 = 9,
    };

    enum class color : uint8_t {
        SELF_COLOR = 0,
        YELLOW     = 1,
        GREEN      = 2,
        ORANGE     = 3,
        PURPLE     = 4,
        PINK       = 5,
        CYAN       = 6,
        BLACK      = 7,
        WHITE      = 8,
    };

    struct line {
        uint16_t end_x = 0;
        uint16_t end_y = 0;

        void set(basic_graphic* graphic) const {
            graphic->details_a = 0;
            graphic->details_b = 0;
            graphic->details_c = 0;
            graphic->details_d = end_x;
            graphic->details_e = end_y;
        }
    };

    struct rect {
        uint16_t opposed_x = 0;
        uint16_t opposed_y = 0;

        void set(basic_graphic* graphic) const {
            graphic->details_a = 0;
            graphic->details_b = 0;
            graphic->details_c = 0;
            graphic->details_d = opposed_x;
            graphic->details_e = opposed_y;
        }
    };

    struct circle {
        uint16_t radius = 0;
        void set(basic_graphic* graphic) const {
            graphic->details_a = 0;
            graphic->details_b = 0;
            graphic->details_c = 0;
            graphic->details_d = radius;
            graphic->details_e = 0;
        }
    };

    struct ellipse {
        uint16_t radius_x = 0;
        uint16_t radius_y = 0;

        void set(basic_graphic* graphic) const {
            graphic->details_a = 0;
            graphic->details_b = 0;
            graphic->details_c = 0;
            graphic->details_d = radius_x;
            graphic->details_e = radius_y;
        }
    };

    struct arc {
        uint16_t radius   = 0;
        uint16_t angle    = 0;
        uint16_t radius_x = 0;
        uint16_t radius_y = 0;

        void set(basic_graphic* graphic) const {
            graphic->details_a = 0;
            graphic->details_b = 0;
            graphic->details_c = radius;
            graphic->details_d = angle;
            graphic->details_e = radius_x;
        }
    };

    struct float_data {
        uint16_t font_size = 0;
        float data         = 0;

        void set(basic_graphic* graphic) const {
            graphic->details_a = font_size;
            graphic->details_b = 0;
            auto data_int      = static_cast<int32_t>(data * 1000);
            std::memcpy(reinterpret_cast<uint8_t*>(graphic) + 11, &data_int, sizeof(data_int));
        }
    };

    struct int_data {
        uint16_t font_size = 0;
        int32_t data       = 0;

        void set(basic_graphic* graphic) const {
            graphic->details_a = font_size;
            graphic->details_b = 0;
            std::memcpy(reinterpret_cast<uint8_t*>(graphic) + 11, &data, sizeof(data));
        }
    };

    template<uint8_t len> struct string_data {
        uint16_t font_size = 0;
        uint8_t data[len]  = {0};

        void set(basic_graphic* graphic) const {
            graphic->details_a = font_size;
            graphic->details_b = len;
            std::memcpy(reinterpret_cast<uint8_t*>(graphic) + 15, data, len);
        }
    };

    struct frame {
        frame_header header;
        uint16_t cmd_id;
        robot_interaction_data_t data_frame;
        uint16_t crc16;
    } __attribute__((packed));


} // namespace ui_dep

class UI {
public:
    using basic_graphic        = ui_dep::basic_graphic;
    using types                = ui_dep::types;
    using operate_delete_layer = ui_dep::operate_delete_layer;
    using operation            = ui_dep::operation;
    using graphic              = ui_dep::graphic;
    using color                = ui_dep::color;
    using layer                = ui_dep::layer;
    using frame                = ui_dep::frame;


    UI(uint16_t sender_id, uint16_t receiver_id, UART_HandleTypeDef* huart)
        : len(0)
        , num(0)
        , type(types::NONE)
        , uartPlus(huart, 200, 200) {
        ui_frame = reinterpret_cast<frame*>(uartPlus.tx_buffer);
        *ui_frame = {0xA5, 0, 0, 0};
        crc::append_crc8_check_sum(reinterpret_cast<uint8_t*>(&(ui_frame->header)), sizeof(frame_header));
        ui_frame->cmd_id                 = 0x301;
        ui_frame->data_frame.sender_id   = sender_id;
        ui_frame->data_frame.receiver_id = receiver_id;
    }

    [[gnu::always_inline]] inline void lock() {};
    [[gnu::always_inline]] inline void unlock() {};

    void delete_layer(ui_dep::operate_delete_layer delete_layer, uint8_t layer_id);
    void operate_str(const uint8_t* name, operation op, uint8_t layer_id, color col, uint16_t font, uint16_t str_len,
                     uint16_t width, uint16_t start_x, uint16_t start_y, const uint8_t* str);
    // void operate_fig(const uint8_t* name, operation op, graphic graph, uint8_t layer_id, color col, uint16_t detail_a,
    //                  uint16_t detail_b, uint16_t width, uint16_t start_x, uint16_t start_y, uint16_t detail_c,
    //                  uint16_t detail_d, uint16_t detail_e);
    template<ui_dep::ui_item ui_graphic>
    void operate_fig(const uint8_t* name, operation op, graphic graph, layer layer_id, color col, uint16_t width,
                     uint16_t start_x, uint16_t start_y, const ui_graphic& item) {
        lock();
        auto* addr   = reinterpret_cast<basic_graphic *>(&(ui_frame->data_frame.user_data[len]));
        (addr->figure_name)[0] =  name[0];
        (addr->figure_name)[1] =  name[1];
        (addr->figure_name)[2] =  name[2];
        addr->operate_type = static_cast<uint8_t>(op);
        addr->figure_type  = static_cast<uint8_t>(graph);
        addr->layer        = static_cast<uint8_t>(layer_id);
        addr->color        = static_cast<uint8_t>(col);
        addr->width        = width;
        addr->start_x      = start_x;
        addr->start_y      = start_y;
        item.set(addr);
        ++num;

        if constexpr (
            std::is_same_v<ui_graphic, ui_dep::line> || std::is_same_v<ui_graphic, ui_dep::rect> ||
            std::is_same_v<ui_graphic, ui_dep::circle> || std::is_same_v<ui_graphic, ui_dep::ellipse> ||
            std::is_same_v<ui_graphic, ui_dep::arc> || std::is_same_v<ui_graphic, ui_dep::float_data> ||
            std::is_same_v<ui_graphic, ui_dep::int_data>) {
            len  += 15;
            type  = types::FIGURE;
        } else {
            len  += 45;
            type  = types::STRING;
        }

        unlock();
    };

    void update();

    void get_feedback();

    void start_receive();

    frame* ui_frame;
    uint32_t len;
    types type;
    SuperUart uartPlus;
    Count rx_cnt;

private:
    uint8_t num;
    void add_frame_header();
    void clear() {
        len  = 0;
        num  = 0;
        type = types::NONE;
    };

};
extern UI ui;
