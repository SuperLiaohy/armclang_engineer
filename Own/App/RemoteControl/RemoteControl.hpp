//
// Created by lhy on 2024/9/10.
//
#pragma once
#include "array"
#include "Detect/Detect.hpp"
#include "RemoteControl/remote_ctrl_dep.hpp"
#include "Uart/SuperUart.hpp"
#include "Key/Key.hpp"


class RemoteControl {
    friend void ::HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size);
    friend void ::HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef* huart);
    friend void ::HAL_UART_ErrorCallback(UART_HandleTypeDef* huart);
    friend void ::HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);
public:
    RemoteControl(UART_HandleTypeDef* huart, uint16_t buffer_size = 60, uint32_t maxInterval = 1000)
        : detect(maxInterval)
        , uartPlus(huart, buffer_size, 0) {}

    void start() { uartPlus.receive_dma_idle(60); };

    void update(KeyBoard& key_board);

    void clear() {
        memset(&rcInfo, 0, sizeof(rcInfo));
        memset(uartPlus.rx_buffer, 0, sizeof(uartPlus.rx_size));
    };

    remote_ctrl_dep::RC_TypeDef rcInfo {};

    std::array<float, 3> pos = {85.912, 0, 241.753};

    Detect detect;

private:
    SuperUart uartPlus;
};

void remote_ctrl_recover();

//extern RemoteControl remote_control;
