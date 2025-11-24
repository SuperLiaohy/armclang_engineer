//
// Created by Administrator on 24-10-3.
//

#include "Interact/Interact.hpp"
#include "ThreadConfig.h"
#include "usart.h"

extern "C" {
extern osThreadId ERROR_TASKHandle;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    UNUSED(huart);

}

static uint8_t cnt = 0;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    UNUSED(Size);
    UNUSED(huart);

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    UNUSED(huart);
};

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart) {
    UNUSED(huart);

};
