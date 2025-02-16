//
// Created by Administrator on 25-1-20.
//

#include "Key.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#ifdef __cplusplus
}
#endif


void KeyBoardRegister(Key *keyList, uint16_t key, KeyCombineType combine,
                      KeyCallbackFunc callback) {
    for (uint8_t index = 0; index < KEY_NUM; index++) {
        if (key & (0x01 << index)) {
            //根据按键事件注册回调
            keyList[index].callback.push_back({combine, callback});
        }
    }
}

void KeyBoardUpdate(Key *keyList, KeyBoard& key_board) {
    static uint32_t lastUpdateTime;
    uint32_t presentTime = HAL_GetTick();

    //检查组合键
    KeyCombineType combineKey = CombineKey_None;
    if (key_board.kb.bit.CTRL)
        combineKey = CombineKey_Ctrl;
    else if (key_board.kb.bit.SHIFT)
        combineKey = CombineKey_Shift;

    for (uint8_t key = 0; key < 18; key++) {
        if (keyList[key].callback.empty()) continue;
        //读取按键状态
        uint8_t thisState = 0;
        if (key < 16)
            thisState = (key_board.kb.key_code & (0x01 << key)) ? 1 : 0; //取出键盘对应位
        else if (key == 16)
            thisState = key_board.mouse.l;
        else if (key == 17)
            thisState = key_board.mouse.r;
        //没有注册回调函数
        uint16_t lastPressTime = lastUpdateTime - keyList[key].startPressTime; //上次更新时按下的时间
        uint16_t pressTime = presentTime - keyList[key].startPressTime;    //当前按下的时间
        //按键状态判定
        /*******按下的一瞬间********/
        if (!keyList[key].lastState && thisState) {
            keyList[key].startPressTime = presentTime;     //记录按下时间
            keyList[key].state = KeyEvent_OnDown; //设置按下事件

        }
            /*******松开的一瞬间********/
        else if (keyList[key].lastState && !thisState) {
            //按键抬起
            keyList[key].state = KeyEvent_OnUp;
            //单击判定
            if (pressTime > keyList[key].clickDelayTime && pressTime < keyList[key].longPressTime) {
                keyList[key].state = KeyEvent_OnClick;
            }
        }
            /*******按键持续按下********/
        else if (keyList[key].lastState && thisState) {
            //执行一直按下的事件回调
            if (pressTime > keyList[key].clickDelayTime) {
                keyList[key].state = KeyEvent_OnPressing;
            }
            //长按判定
            if (lastPressTime <= keyList[key].longPressTime && pressTime > keyList[key].longPressTime) {
                keyList[key].state = KeyEvent_OnLongPress;
            }
        }
            /*******按键持续松开********/
        else {
            keyList[key].state = KeyEvent_None;
        }
        for (auto &i: keyList[key].callback) {
            if (i.combineKey == combineKey) {
                i.func(keyList[key].state);
            }
        }
        keyList[key].lastState = thisState; //记录按键状态
    }

    lastUpdateTime = presentTime; //记录更新事件
}