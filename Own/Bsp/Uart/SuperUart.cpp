//
// Created by liaohy on 8/26/24.
//

#include "SuperUart.h"
#include "Heap/CustomHeap.h"

SuperUart::SuperUart(UART_HandleTypeDef *_uart, const uint16_t bufferSize, const uint16_t tx_buffer_size) {
    if (bufferSize == 0) {
        rx_buffer = nullptr;
    } else {
        rx_buffer = reinterpret_cast<uint8_t *>(D1Heap.malloc(bufferSize));
    }
    this->uart = _uart;
    if (tx_buffer_size != 0) {
        tx_buffer = reinterpret_cast<uint8_t*>(D1Heap.malloc(tx_buffer_size)) ;
    } else {
        tx_buffer = nullptr;
    }
    len = 0;
    tx_size = tx_buffer_size;
    rx_size = bufferSize;
}

SuperUart::~SuperUart() {
    D1Heap.free(tx_buffer);
}

void SuperUart::write(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(uart, data, size, HAL_MAX_DELAY);
}

void SuperUart::write_it(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit_IT(uart, data, size);
}

void SuperUart::write_dma(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit_DMA(uart, data, size);
}

void SuperUart::print(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, 256, format, args);
    va_end(args);
//        HAL_UART_Transmit(uart, (uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);
    HAL_UART_Transmit_DMA(uart, (uint8_t *) buffer, strlen(buffer));
}

void SuperUart::println(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, 256, format, args);
    va_end(args);
    strcat(buffer, "\r\n");
    HAL_UART_Transmit_DMA(uart, (uint8_t *) buffer, strlen(buffer));
}

void SuperUart::print(int32_t value) {
    print("%d", value);
}

void SuperUart::print(float value, uint8_t precision) {
    print("%.*f", precision, value);
}

/*
 * @note: send tx_buffer data(the member of SuperUart) to uart
 */
void SuperUart::print() {
//        HAL_UART_Transmit(uart, (uint8_t *) tx_buffer, len, HAL_MAX_DELAY);
    HAL_UART_Transmit_DMA(uart, tx_buffer, len);
    len = 0;
}

/*
 * @param data: the pointer to the data which you want to store in tx_buffer
 * @param size: the size of data
 * @note: store the data in tx_buffer
 * @note: the size of data must be 1, 2, 4
 * @note: the data will be stored in tx_buffer
 */
void SuperUart::set_hex(void *data, uint16_t size) {
    switch (size) {
        case 1:
            *(tx_buffer + len) = *(int8_t *) data;
            break;
        case 2:
            *(int16_t *) (tx_buffer + len) = *(int16_t *) data;
            break;
        case 4:
            *(int32_t *) (tx_buffer + len) = *(int32_t *) data;
            break;
        default:
            break;
    }
    len += size;
}

/*
 * @param size: the size of data to read
 * @return: the pointer to the data
 * @note: rx_buffer is the class member of SuperUart, it is a DoubleBuffer object, don't need to another Buffer
 * @note: use interrupt to read data
 */
uint8_t* SuperUart::read_it(uint16_t size) {
    auto pData = rx_buffer;
    HAL_UART_Receive_IT(uart, pData, size);
    return pData;
}

/*
 * @param pData: the pointer to the data
 * @param size: the size of data to read
 * @return: the pointer to the data
 * @note: the rx_buffer is provided by you, you can choose to use which buffer to store the data
 * @note: use interrupt to read data
 */
uint8_t* SuperUart::read_it(uint8_t *pData, uint16_t size) {
    HAL_UART_Receive_IT(uart, pData, size);
    return pData;
}


/*
 * @param size: the size of data to read
 * @return: the pointer to the data
 * @note: the rx_buffer is provided by you, you can choose to use which buffer to store the data
 * @note: use DMA_IDLE to read data
 */
uint8_t* SuperUart::read_idle(uint16_t size) {
    auto pData = rx_buffer;
    HAL_UARTEx_ReceiveToIdle_DMA(uart, pData, size);
//        HAL_UARTEx_ReceiveToIdle_IT(uart, pData, size);
    __HAL_DMA_DISABLE_IT(uart->hdmarx, DMA_IT_HT);
    return pData;
}

/*
 * @param pData: the pointer to the data
 * @param size: the size of data to read
 * @return: the pointer to the data
 * @note: the rx_buffer is provided by you, you can choose to use which buffer to store the data
 * @note: use DMA_IDLE to read data
 */
uint8_t *SuperUart::read_idle(uint8_t *pData, uint16_t size) {
    HAL_UARTEx_ReceiveToIdle_DMA(uart, pData, size);
    __HAL_DMA_DISABLE_IT(uart->hdmarx, DMA_IT_HT);
    return pData;
}

/*
 * @param size: the size of data to read
 * @return: the pointer to the data
 * @note: rx_buffer is the class member of SuperUart, it is a DoubleBuffer object, don't need to another Buffer
 */
uint8_t* SuperUart::read(uint16_t size) {
    auto pData = rx_buffer;
    HAL_UART_Receive(uart, pData, size, HAL_MAX_DELAY);
    return pData;
}

/*
 * @param pData: the pointer to the data
 * @param size: the size of data to read
 * @return: the pointer to the data
 * @note: the rx_buffer is provided by you, you can choose to use which buffer to store the data
 */
uint8_t* SuperUart::read(uint8_t *pData, uint16_t size) {
    HAL_UART_Receive(uart, pData, size, HAL_MAX_DELAY);
    return pData;
}


