//
// Created by Administrator on 24-10-1.
//

#include "SuperSPI.h"

void SuperSPI::Write(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit(hspi, pData, Size, 1000);
}

void SuperSPI::Read(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Receive(hspi, pData, Size, 1000);
}

void SuperSPI::WriteIT(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit_IT(hspi, pData, Size);
}

void SuperSPI::ReadIT(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Receive_IT(hspi, pData, Size);
}

void SuperSPI::WriteDMA(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit_DMA(hspi, pData, Size);
}

void SuperSPI::ReadDMA(uint8_t *pData, uint16_t Size) {
    HAL_SPI_Receive_DMA(hspi, pData, Size);
}
