//
// Created by lhy on 2024/9/10.
//

#include "RemoteControl.h"
#include "Buzzer/Buzzer.h"
void remote_ctrl_recover() {
    //    remote_control.clear();
    buzzer.StartMusic(error_music, 8);
    remote_control.start();
}

void RemoteControl::update() {
    auto buff = uartPlus.rx_buffer;

    rcInfo.ch1 = static_cast<int16_t>(((buff[0] | buff[1] << 8) & 0x07FF) - 1024);
    rcInfo.ch2 = static_cast<int16_t>((((buff[1] >> 3 | buff[2] << 5) & 0x07FF) - 1024));
    rcInfo.ch3 = static_cast<int16_t>((((buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF) - 1024));
    rcInfo.ch4 = static_cast<int16_t>((((buff[4] >> 1 | buff[5] << 7) & 0x07FF) - 1024));

    /* prevent remote control zero deviation */
    if (rcInfo.ch1 <= 5 && rcInfo.ch1 >= -5)
        rcInfo.ch1 = 0;
    if (rcInfo.ch2 <= 5 && rcInfo.ch2 >= -5)
        rcInfo.ch2 = 0;
    if (rcInfo.ch3 <= 5 && rcInfo.ch3 >= -5)
        rcInfo.ch3 = 0;
    if (rcInfo.ch4 <= 5 && rcInfo.ch4 >= -5)
        rcInfo.ch4 = 0;

    rcInfo.left  = ((buff[5] >> 4) & 0x000C) >> 2; //sw1   中间是3，上边是1，下边是2
    rcInfo.right = (buff[5] >> 4) & 0x0003;        //sw2

    rcInfo.mouse.x = static_cast<int16_t>((buff[6] | (buff[7] << 8))); // x axis
    rcInfo.mouse.y = static_cast<int16_t>((buff[8] | (buff[9] << 8)));
    rcInfo.mouse.z = static_cast<int16_t>((buff[10] | (buff[11] << 8)));

    rcInfo.mouse.l = buff[12];
    rcInfo.mouse.r = buff[13];

    rcInfo.kb.key_code = buff[14] | buff[15] << 8; // key borad code
    rcInfo.wheel       = static_cast<int16_t>(((buff[16] | buff[17] << 8) - 1024));

    detect.update();
}

void RemoteControl::update(uint8_t* pData) {
    auto buff  = pData;
    rcInfo.ch1 = static_cast<int16_t>(((buff[0] | buff[1] << 8) & 0x07FF) - 1024);
    rcInfo.ch2 = static_cast<int16_t>((((buff[1] >> 3 | buff[2] << 5) & 0x07FF) - 1024));
    rcInfo.ch3 = static_cast<int16_t>((((buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF) - 1024));
    rcInfo.ch4 = static_cast<int16_t>((((buff[4] >> 1 | buff[5] << 7) & 0x07FF) - 1024));

    /* prevent remote control zero deviation */
    if (rcInfo.ch1 <= 5 && rcInfo.ch1 >= -5)
        rcInfo.ch1 = 0;
    if (rcInfo.ch2 <= 5 && rcInfo.ch2 >= -5)
        rcInfo.ch2 = 0;
    if (rcInfo.ch3 <= 5 && rcInfo.ch3 >= -5)
        rcInfo.ch3 = 0;
    if (rcInfo.ch4 <= 5 && rcInfo.ch4 >= -5)
        rcInfo.ch4 = 0;

    rcInfo.left  = ((buff[5] >> 4) & 0x000C) >> 2; //sw1   中间是3，上边是1，下边是2
    rcInfo.right = (buff[5] >> 4) & 0x0003;        //sw2

    rcInfo.mouse.x = static_cast<int16_t>((buff[6] | (buff[7] << 8))); // x axis
    rcInfo.mouse.y = static_cast<int16_t>((buff[8] | (buff[9] << 8)));
    rcInfo.mouse.z = static_cast<int16_t>((buff[10] | (buff[11] << 8)));

    rcInfo.mouse.l = buff[12];
    rcInfo.mouse.r = buff[13];

    rcInfo.kb.key_code = buff[14] | buff[15] << 8; // key borad code
    rcInfo.wheel       = static_cast<int16_t>(((buff[16] | buff[17] << 8) - 1024));

    detect.update();
}

void RemoteControl::KeyBoardUpdate() {
    static uint32_t lastUpdateTime;
    uint32_t presentTime = HAL_GetTick();

    //检查组合键
    KeyCombineType combineKey = CombineKey_None;
    if (rcInfo.kb.bit.CTRL)
        combineKey = CombineKey_Ctrl;
    else if (rcInfo.kb.bit.SHIFT)
        combineKey = CombineKey_Shift;

    for (uint8_t key = 0; key < 18; key++) {
        if (keyList[key].callback.empty()) continue;
        //读取按键状态
        uint8_t thisState = 0;
        if (key < 16)
            thisState = (rcInfo.kb.key_code & (0x01 << key)) ? 1 : 0; //取出键盘对应位
        else if (key == 16)
            thisState = rcInfo.mouse.l;
        else if (key == 17)
            thisState = rcInfo.mouse.r;
                          //没有注册回调函数
        uint16_t lastPressTime = lastUpdateTime - keyList[key].startPressTime; //上次更新时按下的时间
        uint16_t pressTime     = presentTime - keyList[key].startPressTime;    //当前按下的时间
        //按键状态判定
        /*******按下的一瞬间********/
        if (!keyList[key].lastState && thisState) {
            keyList[key].startPressTime = presentTime;     //记录按下时间
            keyList[key].state          = KeyEvent_OnDown; //设置按下事件

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
        for (auto& i: keyList[key].callback) {
            if (i.combineKey == combineKey) {
                i.func(keyList[key].state);
            }
        }
        keyList[key].lastState = thisState; //记录按键状态
    }

    lastUpdateTime = presentTime; //记录更新事件
}

void RemoteControl::KeyBoardRegister(uint16_t key, KeyCombineType combine,
                                     KeyCallbackFunc callback) {
    for (uint8_t index = 0; index < KEY_NUM; index++) {
        if (key & (0x01 << index)) {
            //根据按键事件注册回调
            keyList[index].callback.push_back({combine, callback});
        }
    }
}
