//
// Created by lhy on 2024/9/10.
//
#pragma once

#include "Detect/Detect.hpp"
#include "RemoteControl/remote_ctrl_dep.hpp"

class RemoteControl {
public:
    RemoteControl(UART_HandleTypeDef *huart, uint16_t buffer_size = 60, uint32_t maxInterval = 1000)
            : detect(maxInterval), uartPlus(huart, buffer_size, 0) {
    }

    void start();

    void update(KeyBoard &key_board);

    inline void clear();

    friend void::HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

    friend void::HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

    RC_TypeDef rcInfo{};

    Detect detect;
private:

    SuperUart uartPlus;
};

inline void RemoteControl::start() { uartPlus.receive_dma_idle(60); }

inline void RemoteControl::clear() {
    memset(&rcInfo, 0, sizeof(rcInfo));
    memset(uartPlus.rx_buffer, 0, sizeof(uartPlus.rx_size));
}

void remote_ctrl_recover();

//extern RemoteControl remote_control;

