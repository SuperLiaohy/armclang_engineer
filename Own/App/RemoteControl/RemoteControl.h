//
// Created by lhy on 2024/9/10.
//

#ifndef NONE_PRJ_REMOTECONTROL_H
#define NONE_PRJ_REMOTECONTROL_H

#include "Detect/Detect.h"
#include "RemoteControl/remote_ctrl_dep.h"

class RemoteControl {
public:
    RemoteControl(UART_HandleTypeDef *huart, uint16_t buffer_size, uint32_t maxInterval = 1000)
            : detect(maxInterval), uartPlus(huart, buffer_size, 1), status(remote_ctrl_dep::NORMAL) {
    }

    void start();

    void start(uint8_t *pData);

    void update();

    void update(uint8_t *pData);

    void KeyBoardRegister(uint16_t key, KeyCombineType combine, KeyCallbackFunc callback);

    void KeyBoardUpdate();

    friend void::HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

    friend void::HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

    inline void clear();

    RC_TypeDef rcInfo{};

    Key keyList[KEY_NUM];

    remote_ctrl_dep::status status;

    Detect detect;
private:

    SuperUart uartPlus;
};

inline void RemoteControl::start() {
    uartPlus.read_idle(60);
}

inline void RemoteControl::start(uint8_t *pData) {
    uartPlus.read_idle(pData, 30);
}

inline void RemoteControl::clear() {
    memset(&rcInfo, 0, sizeof(rcInfo));
    memset(uartPlus.rx_buffer, 0, sizeof(uartPlus.rx_size));
    status = remote_ctrl_dep::NORMAL;
}

void remote_ctrl_recover();

extern RemoteControl remote_control;

#endif //NONE_PRJ_REMOTECONTROL_H
