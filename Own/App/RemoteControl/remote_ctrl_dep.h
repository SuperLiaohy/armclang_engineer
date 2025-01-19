//
// Created by lhy on 2024/9/15.
//

#ifndef NEW_PRJ_RC_DEP_H
#define NEW_PRJ_RC_DEP_H

#include "CustomList/CustomList.h"
#include "Uart/SuperUart.h"

#define MAX_KEY_CALLBACK_NUM 10

#define MAX_KEY_COMBINE_NUM 10
#define KEY_NUM 18

namespace remote_ctrl_dep {
    [[gnu::always_inline]] inline float addSpeed(float rc_ch, float max_speed) {
        return rc_ch * max_speed / 660.f;
    }
    enum status {
        LOST,
        KEYBOARD,
        NORMAL
    };
} // namespace remote_ctrl


union ctrl_t {
    struct ctrl_state_t {
        uint8_t none: 2;
        uint8_t reset: 1;
        uint8_t normal: 1;
        uint8_t xyz: 1;
        uint8_t custom: 1;
        uint8_t vision: 1;
        uint8_t key_board: 1;
    } state;
    uint8_t byte;

    [[gnu::always_inline]] inline void set_reset();

    [[gnu::always_inline]] inline void set_normal();

    [[gnu::always_inline]] inline void set_xyz();

    [[gnu::always_inline]] inline void set_vision();

    [[gnu::always_inline]] inline void set_custom();

    [[gnu::always_inline]] inline void set_key_board();
};

inline void ctrl_t::set_reset() { byte = (1 << 2); }

inline void ctrl_t::set_normal() { byte = (1 << 3); }

inline void ctrl_t::set_xyz() { byte = (1 << 4); }

inline void ctrl_t::set_custom() { byte = (1 << 5); }

inline void ctrl_t::set_vision() { byte = (1 << 6); }

inline void ctrl_t::set_key_board() { byte = (1 << 7); }


enum KeyType {
    Key_W = 1 << 0,
    Key_S = 1 << 1,
    Key_A = 1 << 2,
    Key_D = 1 << 3,
    Key_Shift = 1 << 4,
    Key_Ctrl = 1 << 5,
    Key_Q = 1 << 6,
    Key_E = 1 << 7,
    Key_R = 1 << 8,
    Key_F = 1 << 9,
    Key_G = 1 << 10,
    Key_Z = 1 << 11,
    Key_X = 1 << 12,
    Key_C = 1 << 13,
    Key_V = 1 << 14,
    Key_B = 1 << 15,
    //鼠标左右键
    Key_Left = 1 << 16,
    Key_Right = 1 << 17,
    Key_All = 0x3ffff
};

enum KeyEventType {
    KeyEvent_OnClick,
    KeyEvent_OnLongPress,
    KeyEvent_OnDown,
    KeyEvent_OnUp,
    KeyEvent_OnPressing,
    KeyEvent_None,
};

enum KeyCombineType {
    CombineKey_None,
    CombineKey_Ctrl,
    CombineKey_Shift,
};

struct RC_TypeDef {
    /* rocker channel information */
    int16_t ch1;
    int16_t ch2;
    int16_t ch3;
    int16_t ch4;
    /* left and right lever information */
    uint8_t left;
    uint8_t right;
    /* mouse movement and button information */
    struct {
        int16_t x;
        int16_t y;
        int16_t z;

        uint8_t l;
        uint8_t r;
    } mouse;
    /* keyboard key information */
    union {
        uint16_t key_code;
        struct {
            uint16_t W: 1;
            uint16_t S: 1;
            uint16_t A: 1;
            uint16_t D: 1;
            uint16_t SHIFT: 1;
            uint16_t CTRL: 1;
            uint16_t Q: 1;
            uint16_t E: 1;
            uint16_t R: 1;
            uint16_t F: 1;
            uint16_t G: 1;
            uint16_t Z: 1;
            uint16_t X: 1;
            uint16_t C: 1;
            uint16_t V: 1;
            uint16_t B: 1;
        } bit;
    } kb;
    int16_t wheel;
};

typedef void (*KeyCallbackFunc)(KeyEventType event);

// typedef std::function<void()> KeyCallbackFunc;
struct Key {
    //需要配置的参数
    uint16_t clickDelayTime; //按下多久才算单击一次
    uint16_t longPressTime;  //按下多久才算长按

    //用来使用的参数，仅在对应条件有效的一瞬间为1
    KeyEventType state;

    //回调
    struct key_callback {
        KeyCombineType combineKey; //组合键类型列表
        KeyCallbackFunc func;      //回调函数列表
    }; //四种按键事件的回调
    CustomList<key_callback> callback;
    //中间变量
    uint8_t lastState; //1/0为按下/松开
    uint32_t startPressTime;

    explicit Key(uint32_t clickTime = 100, uint32_t longPressTime = 1000)
            : clickDelayTime(clickTime), longPressTime(longPressTime), state(), lastState(0), startPressTime(0) {}
};

#endif //NEW_PRJ_RC_DEP_H
