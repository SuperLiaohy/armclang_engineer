//
// Created by lhy on 2024/9/10.
//
#pragma once
#include "array"
#include "Detect/Detect.hpp"
#include "Uart/SuperUart.hpp"
#include "Key/Key.hpp"


class RemoteControl {
public:
    [[gnu::always_inline]] inline static float addSpeed(float rc_ch, float max_speed) {
        return rc_ch * max_speed / 660.f;
    }

    enum class lever {
        upper = 1,
        middle = 3,
        lower = 2,
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
        int16_t wheel;
    };

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

    RC_TypeDef rcInfo {};

    std::array<float, 3> pos = {85.912, 0, 241.753};

    Detect detect;

private:
    SuperUart uartPlus;
};

void remote_ctrl_recover();

//extern RemoteControl remote_control;
