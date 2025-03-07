//
// Created by liaohy on 8/26/24.
//
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
#ifdef __cplusplus
}
#endif

#define USING_UART_RAW 0
#define USING_UART_IT 0
#define USING_UART_IDLE 1


class SuperUart {
private:
    UART_HandleTypeDef *uart;
    int32_t len;

public:

    uint16_t tx_size;
    uint16_t rx_size;

    uint8_t *rx_buffer;

    explicit SuperUart(UART_HandleTypeDef *_uart, uint16_t bufferSize, uint16_t tx_buffer_size = 64);

    ~SuperUart();

    void transmit_pdata(uint8_t *data, uint16_t size);

    void transmit_it_pdata(uint8_t *data, uint16_t size);

    void transmit_dma_pdata(uint8_t *data, uint16_t size);

    void print(const char *format, ...);

    void println(const char *format, ...);

    void print(int32_t value);

    void print(float value, uint8_t precision = 2);

    //print hex data,send tx_buffer data(the member of SuperUart)
    void print();

    //store hex data to tx_buffer
    void set_hex(void *data, uint16_t size);

    void transmit(uint16_t size);
    //use read api to read data from rx_buffer (mode : BLOCK)
    uint8_t* receive(uint16_t size);

    uint8_t* receive(uint8_t *pData, uint16_t size);

    //use read_it api to read data from rx_buffer (mode : INTERRUPT)
    uint8_t* receive_it(uint16_t size);

    uint8_t* receive_it(uint8_t *pData, uint16_t size);

    //use read_idle api to read data from rx_buffer (mode : DMA_IDLE)
    uint8_t* receive_dma_idle(uint16_t size);

    uint8_t* receive_dma_idle(uint8_t *pData, uint16_t size);

    friend void::HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

    friend void::HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

    friend void::HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);

    friend void::HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart);

    uint8_t *tx_buffer;
};

extern SuperUart uartPlus10;

