//
// Created by Administrator on 25-1-20.
//

#ifndef ARMCLANG_ENGINEER_KEY_H
#define ARMCLANG_ENGINEER_KEY_H

#include "CustomList/CustomList.h"
#include "cstdint"

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

struct KeyBoard {
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

    explicit Key(uint32_t clickTime = 75, uint32_t longPressTime = 1000)
            : clickDelayTime(clickTime), longPressTime(longPressTime), state(), lastState(0), startPressTime(0) {}
};
void KeyBoardRegister(Key *keyList, uint32_t key, KeyCombineType combine, KeyCallbackFunc callback);
void KeyBoardUpdate(Key *keyList, KeyBoard& key_board);
#define KEY_NUM 18
//extern Key keyList[KEY_NUM];
//extern KeyBoard key_board;
#endif //ARMCLANG_ENGINEER_KEY_H
