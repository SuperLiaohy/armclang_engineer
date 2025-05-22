#pragma once

#include "referee_system.h"

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
    union {
        struct {
            uint32_t details_c : 10;
            uint32_t details_d : 11;
            uint32_t details_e : 11;
        };
        int32_t data;
        const char* str;
    };
} __attribute__((packed));

template<typename T>
concept ui_features = requires(T t) { t.set_features(std::declval<basic_graphic*>()); };

class UI {
public:
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
        uint8_t display_interval;
        int8_t display_num;
        bool visible;

        ui_control(const uint8_t freq = 1)
            : display_interval(freq)
            , display_num(1)
            , visible(true) {};
    };

    enum class shape : uint8_t {
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

    class ui_item {
    public:
        basic_graphic graphic;
        ui_control control;

    protected:
        template<typename T> bool is_change(int32_t self, T t) {
            if (static_cast<int32_t>(t) != (self)) {
                control.visible = true;
                return true;
            }
            return false;
        };

    private:
        void set_name(const char* name) {
            graphic.figure_name[0] = name[0];
            graphic.figure_name[1] = name[1];
            graphic.figure_name[2] = name[2];
        };
    public:
        void set_operate(operation op) {
            if (is_change(graphic.operate_type, op)) graphic.operate_type = static_cast<uint8_t>(op);
        };

    private:
        void set_shape(shape s) {
            if (is_change(graphic.figure_type, s)) graphic.figure_type = static_cast<uint8_t>(s);
        };

    public:
        void set_layer(const layer l) {
            if (is_change(graphic.layer, l)) graphic.layer = static_cast<uint8_t>(l);
        };
        void set_color(const color c) {
            if (is_change(graphic.color, c)) graphic.color = static_cast<uint8_t>(c);
        };
        void set_line_width(const uint16_t line_width) {
            if (is_change(graphic.width, line_width)) graphic.width = line_width;
        };
        void set_startX(const uint16_t start_x) {
            if (is_change(graphic.start_x, start_x)) graphic.start_x = start_x;
        };
        void set_startY(const uint8_t start_y) {
            if (is_change(graphic.start_y, start_y)) graphic.start_y = start_y;
        };
    };

    class line : public ui_item {
    public:
        void set_endX(const uint16_t end_x) {
            if (is_change(graphic.details_d, end_x)) graphic.details_d = end_x;
        }
        void set_endY(const uint16_t end_y) {
            if (is_change(graphic.details_e, end_y)) graphic.details_e = end_y;
        }
    };

    class rect : public ui_item {
    public:
        void set_endX(const uint16_t end_x) {
            if (is_change(graphic.details_d, end_x)) graphic.details_d = end_x;
        }
        void set_endY(const uint16_t end_y) {
            if (is_change(graphic.details_e, end_y)) graphic.details_e = end_y;
        }
    };

    class circle : public ui_item {
    public:
        void set_radius(const uint16_t r) {
            if (is_change(graphic.details_c, r)) graphic.details_c = r;
        }
    };

    class ellipse : public ui_item {
    public:
        void set_radiusX(const uint16_t r) {
            if (is_change(graphic.details_d, r)) graphic.details_d = r;
        }
        void set_radiusY(const uint16_t r) {
            if (is_change(graphic.details_e, r)) graphic.details_e = r;
        }
    };

    class arc : public ui_item {
    public:
        void set_start_angle(const uint16_t s) {
            if (is_change(graphic.details_a, s)) graphic.details_a = s;
        }
        void set_end_angle(const uint16_t e) {
            if (is_change(graphic.details_b, e)) graphic.details_b = e;
        }
        void set_radiusX(const uint16_t r) {
            if (is_change(graphic.details_d, r)) graphic.details_d = r;
        }
        void set_radiusY(const uint16_t r) {
            if (is_change(graphic.details_e, r)) graphic.details_e = r;
        }
    };

    class float_data : public ui_item {
    public:
        void set_font_size(const uint16_t s) {
            if (is_change(graphic.details_a, s)) graphic.details_a = s;
        };
        void set_float(const float f) {
            if (const int32_t data = f * 1000; is_change(graphic.data, data)) graphic.data = data;
        };
    };

    class int_data : public ui_item {
    public:
        void set_font_size(const uint16_t s) {
            if (is_change(graphic.details_a, s)) graphic.details_a = s;
        };
        void set_int(const int32_t i) {
            if (is_change(graphic.data, i)) graphic.data = i;
        };
    };

    class string_data : public ui_item {
    public:
        void set_font_size(const uint16_t s) {
            if (is_change(graphic.details_a, s)) graphic.details_a = s;
        };
        void set_len(const uint8_t i) {
            if (is_change(graphic.details_b, i)) graphic.details_b = i;
        };
        void set_str(const char* str) {
            graphic.str     = str;
            control.visible = true;
        }
    };

    static ui_item figure_list[30];
    static uint8_t figure_index;

    static ui_item str_list[30];
    static uint8_t str_index;

    static line* create_line(const char* name, const operation op, const layer l, const color c,
                             const uint16_t line_width, const uint16_t start_x, const uint16_t start_y,
                             const uint16_t end_x, const uint16_t end_y, const uint8_t freq) {
        const auto ptr              = reinterpret_cast<line*>(&figure_list[figure_index++]);
        ptr->graphic.figure_name[0] = name[0];
        ptr->graphic.figure_name[1] = name[1];
        ptr->graphic.figure_name[2] = name[2];

        ptr->graphic.operate_type = static_cast<uint8_t>(op);
        ptr->graphic.figure_type  = static_cast<uint8_t>(shape::LINE);
        ptr->graphic.layer        = static_cast<uint8_t>(l);
        ptr->graphic.color        = static_cast<uint8_t>(c);
        ptr->graphic.width        = line_width;
        ptr->graphic.start_x      = start_x;
        ptr->graphic.start_y      = start_y;

        ptr->set_endX(end_x);
        ptr->set_endY(end_y);

        ptr->control.display_interval = freq;
        ptr->control.display_num      = 1;
        ptr->control.visible          = true;
        return ptr;
    }
    static rect* create_rect(const char* name, const operation op, const layer l, const color c,
                             const uint16_t line_width, const uint16_t start_x, const uint16_t start_y,
                             const uint16_t end_x, const uint16_t end_y, const uint8_t freq) {
        const auto ptr              = reinterpret_cast<rect*>(&figure_list[figure_index++]);
        ptr->graphic.figure_name[0] = name[0];
        ptr->graphic.figure_name[1] = name[1];
        ptr->graphic.figure_name[2] = name[2];

        ptr->graphic.operate_type = static_cast<uint8_t>(op);
        ptr->graphic.figure_type  = static_cast<uint8_t>(shape::RECT);
        ptr->graphic.layer        = static_cast<uint8_t>(l);
        ptr->graphic.color        = static_cast<uint8_t>(c);
        ptr->graphic.width        = line_width;
        ptr->graphic.start_x      = start_x;
        ptr->graphic.start_y      = start_y;

        ptr->set_endX(end_x);
        ptr->set_endY(end_y);

        ptr->control.display_interval = freq;
        ptr->control.display_num      = 1;
        ptr->control.visible          = true;
        return ptr;
    }
    static circle* create_circle(const char* name, const operation op, const layer l, const color c,
                                 const uint16_t line_width, const uint16_t start_x, const uint16_t start_y,
                                 const uint16_t radius, const uint8_t freq) {
        const auto ptr              = reinterpret_cast<circle*>(&figure_list[figure_index++]);
        ptr->graphic.figure_name[0] = name[0];
        ptr->graphic.figure_name[1] = name[1];
        ptr->graphic.figure_name[2] = name[2];

        ptr->graphic.operate_type = static_cast<uint8_t>(op);
        ptr->graphic.figure_type  = static_cast<uint8_t>(shape::CIRCLE);
        ptr->graphic.layer        = static_cast<uint8_t>(l);
        ptr->graphic.color        = static_cast<uint8_t>(c);
        ptr->graphic.width        = line_width;
        ptr->graphic.start_x      = start_x;
        ptr->graphic.start_y      = start_y;

        ptr->set_radius(radius);

        ptr->control.display_interval = freq;
        ptr->control.display_num      = 1;
        ptr->control.visible          = true;
        return ptr;
    }
    static ellipse* create_ellipse(const char* name, const operation op, const layer l, const color c,
                                   const uint16_t line_width, const uint16_t start_x, const uint16_t start_y,
                                   const uint16_t radius_x, const uint16_t radius_y, const uint8_t freq) {
        const auto ptr              = reinterpret_cast<ellipse*>(&figure_list[figure_index++]);
        ptr->graphic.figure_name[0] = name[0];
        ptr->graphic.figure_name[1] = name[1];
        ptr->graphic.figure_name[2] = name[2];

        ptr->graphic.operate_type = static_cast<uint8_t>(op);
        ptr->graphic.figure_type  = static_cast<uint8_t>(shape::ELLIPSE);
        ptr->graphic.layer        = static_cast<uint8_t>(l);
        ptr->graphic.color        = static_cast<uint8_t>(c);
        ptr->graphic.width        = line_width;
        ptr->graphic.start_x      = start_x;
        ptr->graphic.start_y      = start_y;

        ptr->set_radiusX(radius_x);
        ptr->set_radiusY(radius_y);

        ptr->control.display_interval = freq;
        ptr->control.display_num      = 1;
        ptr->control.visible          = true;
        return ptr;
    }
    static arc* create_arc(const char* name, const operation op, const layer l, const color c,
                           const uint16_t line_width, const uint16_t start_x, const uint16_t start_y,
                           const uint16_t start_angle, const uint16_t end_angle, const uint16_t radius_x,
                           const uint16_t radius_y, const uint8_t freq) {
        const auto ptr              = reinterpret_cast<arc*>(&figure_list[figure_index++]);
        ptr->graphic.figure_name[0] = name[0];
        ptr->graphic.figure_name[1] = name[1];
        ptr->graphic.figure_name[2] = name[2];

        ptr->graphic.operate_type = static_cast<uint8_t>(op);
        ptr->graphic.figure_type  = static_cast<uint8_t>(shape::ARC);
        ptr->graphic.layer        = static_cast<uint8_t>(l);
        ptr->graphic.color        = static_cast<uint8_t>(c);
        ptr->graphic.width        = line_width;
        ptr->graphic.start_x      = start_x;
        ptr->graphic.start_y      = start_y;

        ptr->set_start_angle(start_angle);
        ptr->set_end_angle(end_angle);
        ptr->set_radiusX(radius_x);
        ptr->set_radiusY(radius_y);

        ptr->control.display_interval = freq;
        ptr->control.display_num      = 1;
        ptr->control.visible          = true;
        return ptr;
    }
    static float_data* create_float(const char* name, const operation op, const layer l, const color c,
                                    const uint16_t line_width, const uint16_t start_x, const uint16_t start_y,
                                    const uint16_t font_size, const float data, const uint8_t freq) {
        const auto ptr              = reinterpret_cast<float_data*>(&figure_list[figure_index++]);
        ptr->graphic.figure_name[0] = name[0];
        ptr->graphic.figure_name[1] = name[1];
        ptr->graphic.figure_name[2] = name[2];

        ptr->graphic.operate_type = static_cast<uint8_t>(op);
        ptr->graphic.figure_type  = static_cast<uint8_t>(shape::FLOAT);
        ptr->graphic.layer        = static_cast<uint8_t>(l);
        ptr->graphic.color        = static_cast<uint8_t>(c);
        ptr->graphic.width        = line_width;
        ptr->graphic.start_x      = start_x;
        ptr->graphic.start_y      = start_y;

        ptr->set_font_size(font_size);
        ptr->set_float(data);

        ptr->control.display_interval = freq;
        ptr->control.display_num      = 1;
        ptr->control.visible          = true;
        return ptr;
    }
    static int_data* create_int(const char* name, const operation op, const layer l, const color c,
                                const uint16_t line_width, const uint16_t start_x, const uint16_t start_y,
                                const uint16_t font_size, const int32_t data, const uint8_t freq) {
        const auto ptr              = reinterpret_cast<int_data*>(&figure_list[figure_index++]);
        ptr->graphic.figure_name[0] = name[0];
        ptr->graphic.figure_name[1] = name[1];
        ptr->graphic.figure_name[2] = name[2];

        ptr->graphic.operate_type = static_cast<uint8_t>(op);
        ptr->graphic.figure_type  = static_cast<uint8_t>(shape::INT);
        ptr->graphic.layer        = static_cast<uint8_t>(l);
        ptr->graphic.color        = static_cast<uint8_t>(c);
        ptr->graphic.width        = line_width;
        ptr->graphic.start_x      = start_x;
        ptr->graphic.start_y      = start_y;

        ptr->set_font_size(font_size);
        ptr->set_int(data);

        ptr->control.display_interval = freq;
        ptr->control.display_num      = 1;
        ptr->control.visible          = true;

        return ptr;
    }
    static string_data* create_str(const char* name, const operation op, const layer l, const color c,
                                   const uint16_t line_width, const uint16_t start_x, const uint16_t start_y,
                                   const uint16_t font_size, const uint16_t len, const char* str, const uint8_t freq) {
        const auto ptr              = reinterpret_cast<string_data*>(&str_list[str_index++]);
        ptr->graphic.figure_name[0] = name[0];
        ptr->graphic.figure_name[1] = name[1];
        ptr->graphic.figure_name[2] = name[2];

        ptr->graphic.operate_type = static_cast<uint8_t>(op);
        ptr->graphic.figure_type  = static_cast<uint8_t>(shape::STR);
        ptr->graphic.layer        = static_cast<uint8_t>(l);
        ptr->graphic.color        = static_cast<uint8_t>(c);
        ptr->graphic.width        = line_width;
        ptr->graphic.start_x      = start_x;
        ptr->graphic.start_y      = start_y;

        ptr->set_font_size(font_size);
        ptr->set_len(len);
        ptr->set_str(str);

        ptr->control.display_interval = freq;
        ptr->control.display_num      = 1;
        ptr->control.visible          = true;
        return ptr;
    }

    struct frame {
        frame_header header;
        uint16_t cmd_id;
        robot_interaction_data_t data_frame;
        uint16_t crc16;
    } __attribute__((packed));

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

    void delete_layer(operate_delete_layer delete_layer, uint8_t layer_id);

    bool update();

    void get_feedback();

    void start_receive();

    frame* ui_frame;
    uint32_t len;
    types type;
    SuperUart uartPlus;
    Count rx_cnt;
    void clear() {
        len  = 0;
        num  = 0;
        type = types::NONE;
    };
    void reset() {
        figure_index = 0;
        str_index    = 0;
        clear();
    }
private:
    uint8_t num;
    void add_frame_header();
};
extern UI ui;
