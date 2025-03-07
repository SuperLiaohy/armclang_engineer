//
// Created by Administrator on 24-10-1.
//

#include "SuperSPI.hpp"

void SuperSPI::transmit(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit(hspi, pData, Size, 1000);
}

void SuperSPI::receive(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Receive(hspi, pData, Size, 1000);
}

void SuperSPI::transmit_it(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit_IT(hspi, pData, Size);
}

void SuperSPI::receive_it(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Receive_IT(hspi, pData, Size);
}

void SuperSPI::transmit_dma(const uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit_DMA(hspi, pData, Size);
}

void SuperSPI::receive_dma(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Receive_DMA(hspi, pData, Size);
}
