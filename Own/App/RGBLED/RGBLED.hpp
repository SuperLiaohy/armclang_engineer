//
// Created by Administrator on 24-10-1.
//
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os.h"

#ifdef __cplusplus
}
#endif

#include "SPI/SuperSPI.hpp"
/**
 * RGBLED使用的时SPI6
 * 而SPI6使用的是DMA2_Stream5
 * 其DMA只能在RAM3中
 * 所以使用SPI DMA传输的数据必须要在RAM3中
 */
class RGBLED {
public:
    explicit RGBLED(SPI_HandleTypeDef* hspi)
        : spiPlus(hspi)
        , red(0)
        , green(0)
        , blue(0) {}

    void SetColor(uint8_t _red, uint8_t _green, uint8_t _blue);

    void SetColor(uint32_t color);

    void SetColor();

    void update();

    enum class delay_pl {
        OS,
        HAL,
    };

private:
    template<delay_pl pl> void delay(uint32_t ms) {
        if constexpr (pl == delay_pl::OS) {
            osDelay(ms);
        } else if constexpr (pl == delay_pl::HAL) {
            HAL_Delay(ms);
        }
    }
    void load();

    uint8_t txbuf[24] {};

    SuperSPI spiPlus;

    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
inline void RGBLED::SetColor() { load(); }

inline void RGBLED::update() {
    SetColor();
    red++;
    green += 5;
    blue  += 10;
    delay<delay_pl::OS>(100);
}

extern RGBLED Led;
