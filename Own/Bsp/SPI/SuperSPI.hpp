//
// Created by Administrator on 24-10-1.
//
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "spi.h"

#ifdef __cplusplus
}
#endif

class SuperSPI {
public:
    explicit SuperSPI(SPI_HandleTypeDef* hspi)
        : hspi(hspi) {}

    void transmit(uint8_t* pData, uint16_t Size) { HAL_SPI_Transmit(hspi, pData, Size, 1000); };
    void transmit_it(uint8_t* pData, uint16_t Size) { HAL_SPI_Transmit_IT(hspi, pData, Size); };
    void transmit_dma(const uint8_t* pData, uint16_t Size) { HAL_SPI_Transmit_DMA(hspi, pData, Size); };

    void receive(uint8_t* pData, uint16_t Size) { HAL_SPI_Receive(hspi, pData, Size, 1000); };
    void receive_it(uint8_t* pData, uint16_t Size) { HAL_SPI_Receive_IT(hspi, pData, Size); };
    void receive_dma(uint8_t* pData, uint16_t Size) { HAL_SPI_Receive_DMA(hspi, pData, Size); };

private:
    SPI_HandleTypeDef* hspi;
};
