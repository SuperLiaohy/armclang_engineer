//
// Created by Administrator on 24-10-1.
//

#ifndef DM_42_SUPERSPI_H
#define DM_42_SUPERSPI_H

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

    void Write(uint8_t *pData, uint16_t Size);
    void WriteIT(uint8_t *pData, uint16_t Size);
    void WriteDMA(uint8_t *pData, uint16_t Size);

    void Read(uint8_t *pData, uint16_t Size);
    void ReadIT(uint8_t *pData, uint16_t Size);
    void ReadDMA(uint8_t *pData, uint16_t Size);
private:
    SPI_HandleTypeDef *hspi;

};

#endif //DM_42_SUPERSPI_H
