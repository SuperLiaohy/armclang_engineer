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
    explicit SuperSPI(SPI_HandleTypeDef *hspi) : hspi(hspi) {}

    void transmit(uint8_t *pData, uint16_t Size);
    void transmit_it(uint8_t *pData, uint16_t Size);
    void transmit_dma(const uint8_t *pData, uint16_t Size);

    void receive(uint8_t *pData, uint16_t Size);
    void receive_it(uint8_t *pData, uint16_t Size);
    void receive_dma(uint8_t *pData, uint16_t Size);
private:
    SPI_HandleTypeDef *hspi;

};

