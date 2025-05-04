#pragma once

#include "referee_system.h"
#include <variant>

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
    concept ui_features = requires(T t) { t.set_features(std::declval<basic_graphic*>()); };

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

    struct ui_control {
        uint8_t name[3];
        uint8_t display_interval;
        uint8_t display_num;
        operation op;
        ui_control(const uint8_t* name, const uint8_t freq) { this->init(name, freq); }
        void init(const uint8_t* name, const uint8_t freq) {
            this->name[0] = name[0];
            this->name[1] = name[1];
            this->name[2] = name[2];
            this->display_interval = freq;
            this->op = operation::ADD;
            // if (freq == 0) {
            //     this->display_interval = -1;
            //     this->display_num      = 0;
            //     return;
            // }
            // if (30 % freq > freq / 2) {
            //     this->display_interval = 30 / freq;
            // } else {
            //     this->display_interval = 30 / freq + 1;
            // }
            this->display_num = 1;
        }
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

        void set_features(basic_graphic* graphic) const {
            graphic->figure_type = static_cast<uint32_t>(graphic::LINE);
            graphic->details_a   = 0;
            graphic->details_b   = 0;
            graphic->details_c   = 0;
            graphic->details_d   = end_x;
            graphic->details_e   = end_y;
        }
    };

    struct rect {
        uint16_t opposed_x = 0;
        uint16_t opposed_y = 0;


        void set_features(basic_graphic* graphic) const {
            graphic->figure_type = static_cast<uint32_t>(graphic::RECT);
            graphic->details_a   = 0;
            graphic->details_b   = 0;
            graphic->details_c   = 0;
            graphic->details_d   = opposed_x;
            graphic->details_e   = opposed_y;
        }
    };

    struct circle {
        uint16_t radius;



        void set_features(basic_graphic* graphic) const {
            graphic->figure_type = static_cast<uint32_t>(graphic::CIRCLE);
            graphic->details_a   = 0;
            graphic->details_b   = 0;
            graphic->details_c   = radius;
            graphic->details_d   = 0;
            graphic->details_e   = 0;
        }
    };

    struct ellipse {
        uint16_t radius_x = 0;
        uint16_t radius_y = 0;



        void set_features(basic_graphic* graphic) const {
            graphic->figure_type = static_cast<uint32_t>(graphic::ELLIPSE);
            graphic->details_a   = 0;
            graphic->details_b   = 0;
            graphic->details_c   = 0;
            graphic->details_d   = radius_x;
            graphic->details_e   = radius_y;
        }
    };

    struct arc {
        uint16_t start_angle = 0;
        uint16_t end_angle   = 0;
        uint16_t radius_x    = 0;
        uint16_t radius_y    = 0;

        void set_features(basic_graphic* graphic) const {
            graphic->figure_type = static_cast<uint32_t>(graphic::ARC);
            graphic->details_a   = start_angle;
            graphic->details_b   = end_angle;
            graphic->details_c   = 0;
            graphic->details_d   = radius_x;
            graphic->details_e   = radius_y;
        }
    };

    struct float_data {
        uint16_t font_size = 0;
        float data        = 0;

        void set_features(basic_graphic* graphic) const {
            graphic->figure_type = static_cast<uint32_t>(graphic::FLOAT);
            graphic->details_a   = font_size;
            graphic->details_b   = 0;
            const auto data_int  = static_cast<int32_t>(data * 1000);
            std::memcpy(reinterpret_cast<uint8_t*>(graphic) + 11, &data_int, sizeof(data_int));
        }
    };

    struct int_data {
        uint16_t font_size = 0;
        int32_t data      = 0;


        void set_features(basic_graphic* graphic) const {
            graphic->figure_type = static_cast<uint32_t>(graphic::INT);
            graphic->details_a   = font_size;
            graphic->details_b   = 0;
            std::memcpy(reinterpret_cast<uint8_t*>(graphic) + 11, &data, sizeof(data));
        }
    };

    struct string_data {
        uint16_t font_size  = 0;
        const uint8_t* data = nullptr;
        uint8_t length      = 0;

        void set_features(basic_graphic* graphic) const {
            graphic->figure_type = static_cast<uint32_t>(graphic::STR);
            graphic->details_a   = font_size;
            graphic->details_b   = length;
            std::memcpy(reinterpret_cast<uint8_t*>(graphic) + 15, data, length);
        }
    };

    template<ui_features T> struct UiItem {
        ui_control control;
        operation op;
        layer layer_id;
        color col;
        uint16_t width;
        uint16_t start_x;
        uint16_t start_y;
        T feature;

        UiItem(const uint8_t* name, const uint8_t freq)
            : control(name, freq) {}
        UiItem(const uint8_t* name, const uint8_t freq, operation op, layer layer_id, color col, uint16_t width,
               uint16_t start_x, uint16_t start_y)
            : control(name, freq)
            , op(op)
            , layer_id(layer_id)
            , col(col)
            , width(width)
            , start_x(start_x)
            , start_y(start_y) {}

        UiItem(const uint8_t* name, const uint8_t freq, operation op, layer layer_id, color col, uint16_t width,
               uint16_t start_x, uint16_t start_y, const T& args)
            : control(name, freq)
            , op(op)
            , layer_id(layer_id)
            , col(col)
            , width(width)
            , start_x(start_x)
            , start_y(start_y)
            , feature(args) {}
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
    using basic_graphic                          = ui_dep::basic_graphic;
    using types                                  = ui_dep::types;
    using operate_delete_layer                   = ui_dep::operate_delete_layer;
    using operation                              = ui_dep::operation;
    using graphic                                = ui_dep::graphic;
    using color                                  = ui_dep::color;
    using layer                                  = ui_dep::layer;
    using frame                                  = ui_dep::frame;
    using ui_control                             = ui_dep::ui_control;
    template<ui_dep::ui_features T> using UiItem = ui_dep::UiItem<T>;
    using VarUiItem = std::variant<UiItem<ui_dep::line>, UiItem<ui_dep::circle>, UiItem<ui_dep::arc>,
                                   UiItem<ui_dep::ellipse>, UiItem<ui_dep::rect>, UiItem<ui_dep::int_data>,
                                   UiItem<ui_dep::float_data>, UiItem<ui_dep::string_data>>;

    UI(uint16_t sender_id, uint16_t receiver_id, UART_HandleTypeDef* huart)
        : len(0)
        , type(types::NONE)
        , uartPlus(huart, 200, 200)
        , num(0) {
        ui_frame                         = reinterpret_cast<frame*>(uartPlus.tx_buffer);
        *ui_frame                        = {0xA5, 0, 0, 0};
        ui_frame->cmd_id                 = 0x301;
        ui_frame->data_frame.sender_id   = sender_id;
        ui_frame->data_frame.receiver_id = receiver_id;
    }

    [[gnu::always_inline]] inline void lock() {};
    [[gnu::always_inline]] inline void unlock() {};

    void delete_layer(ui_dep::operate_delete_layer delete_layer, uint8_t layer_id);

    template<ui_dep::ui_features ui_graphic>
    void operate_fig(const uint8_t* name, operation op, layer layer_id, color col, uint16_t width, uint16_t start_x,
                     uint16_t start_y, const ui_graphic& item) {
        lock();
        auto* addr             = reinterpret_cast<basic_graphic*>(&((ui_frame->data_frame.user_data)[len]));
        (addr->figure_name)[0] = name[0];
        (addr->figure_name)[1] = name[1];
        (addr->figure_name)[2] = name[2];
        addr->operate_type     = static_cast<uint8_t>(op);
        addr->layer            = static_cast<uint8_t>(layer_id);
        addr->color            = static_cast<uint8_t>(col);
        addr->width            = width;
        addr->start_x          = start_x;
        addr->start_y          = start_y;
        item.set_features(addr);
        ++num;

        if constexpr (std::is_same_v<ui_graphic, ui_dep::string_data>) {
            len  += 45;
            type  = types::STRING;
        } else {
            len  += 15;
            type  = types::FIGURE;
        }

        unlock();
    };
    template<ui_dep::ui_features ui_graphic>
    void operate_fig(ui_control& control, layer layer_id, color col, uint16_t width, uint16_t start_x,
                     uint16_t start_y, const ui_graphic& item) {
        lock();
        if (num < 7 && control.display_num > 0) {
            auto* addr             = reinterpret_cast<basic_graphic*>(&((ui_frame->data_frame.user_data)[len]));
            (addr->figure_name)[0] = control.name[0];
            (addr->figure_name)[1] = control.name[1];
            (addr->figure_name)[2] = control.name[2];
            addr->operate_type     = static_cast<uint8_t>(control.op);
            addr->layer            = static_cast<uint8_t>(layer_id);
            addr->color            = static_cast<uint8_t>(col);
            addr->width            = width;
            addr->start_x          = start_x;
            addr->start_y          = start_y;
            item.set_features(addr);
            ++num;
            if constexpr (std::is_same_v<ui_graphic, ui_dep::string_data>) {
                len  += 45;
                type  = types::STRING;
            } else {
                len  += 15;
                type  = types::FIGURE;
            }
            --control.display_num;
        }
        unlock();
    };
    template<ui_dep::ui_features ui_graphic> void operate_fig(UiItem<ui_graphic>& item) {
        lock();

        if (type != types::DELETE && type != types::STRING) {
            if (num < 7 && item.control.display_num > 0) {
                auto* addr             = reinterpret_cast<basic_graphic*>(&((ui_frame->data_frame.user_data)[len]));
                (addr->figure_name)[0] = item.control.name[0];
                (addr->figure_name)[1] = item.control.name[1];
                (addr->figure_name)[2] = item.control.name[2];
                addr->operate_type     = static_cast<uint8_t>(item.op);
                addr->layer            = static_cast<uint8_t>(item.layer_id);
                addr->color            = static_cast<uint8_t>(item.col);
                addr->width            = item.width;
                addr->start_x          = item.start_x;
                addr->start_y          = item.start_y;
                item.feature.set_features(addr);
                ++num;
                if constexpr (std::is_same_v<ui_graphic, ui_dep::string_data>) {
                    len  += 45;
                    type  = types::STRING;
                } else {
                    len  += 15;
                    type  = types::FIGURE;
                }
                --item.control.display_num;
            }
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
