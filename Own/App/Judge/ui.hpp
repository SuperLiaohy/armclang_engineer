#pragma once

#include "referee_system.h"

namespace ui_dep {
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
} // namespace ui_dep

class UI {
public:
    using types                = ui_dep::types;
    using operate_delete_layer = ui_dep::operate_delete_layer;
    using operation            = ui_dep::operation;
    using graphic              = ui_dep::graphic;
    using color                = ui_dep::color;

    uint8_t len;

    struct {
        frame_header header;
        uint16_t cmd_id;
        robot_interaction_data_t data_frame;
        uint16_t crc16;
    } __attribute__((packed)) frame;

    UI(uint16_t sender_id, uint16_t receiver_id) {
        frame = {0xA5, 30, 0, 0};
        crc::append_crc8_check_sum(reinterpret_cast<uint8_t*>(&frame.header), sizeof(frame_header));
        frame.cmd_id = 0x301;
        frame.data_frame.sender_id = sender_id;
        frame.data_frame.receiver_id = receiver_id;
    }

    [[gnu::always_inline]] inline void lock() {};
    [[gnu::always_inline]] inline void unlock() {};

    void delete_layer(ui_dep::operate_delete_layer delete_layer, uint8_t layer_id);
    void operate_str(const uint8_t* name, operation op, uint8_t layer_id, color col, uint16_t font, uint16_t str_len, uint16_t width, uint16_t start_x, uint16_t start_y,const uint8_t *str);
    void operate_fig(uint8_t* name, operation op, graphic graph, uint8_t layer_id, color col, uint16_t detail_a, uint16_t detail_b, uint16_t width, uint16_t start_x, uint16_t start_y, uint16_t detail_c, uint16_t detail_d, uint16_t detail_e);

    void add_frame_header();
    void clear() { len = 0; num = 0; type = types::NONE;};
private:
    uint8_t num;
    types type;
    uint16_t sum;
};
extern UI ui;
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "RobotArm.h"
/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern uint8_t UI_reset_flag;
/* Exported function declarations --------------------------------------------*/
void UI_draw_layer_9(void);
void UI_draw_layer_8(void);
void UI_draw_arm_state(void);
void UI_draw_sucker_state(void);
void UI_draw_layer_5(void);
