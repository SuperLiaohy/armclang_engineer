//
// Created by Administrator on 24-10-1.
//

#ifndef DM_42_RGBLED_H
#define DM_42_RGBLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_os.h"

#ifdef __cplusplus
}
#endif

#include "SPI/SuperSPI.h"
/*
 * RGBLED使用的时SPI6
 * 而SPI6使用的是DMA2_Stream5
 * 其DMA只能在RAM3中
 * 所以使用SPI DMA传输的数据必须要在RAM3中
 */
class RGBLED {
public:
    explicit RGBLED(SPI_HandleTypeDef *hspi)
            : spiPlus(hspi),
              red(0), green(0), blue(0) {}

    void SetColor(uint8_t _red, uint8_t _green, uint8_t _blue);
    void SetColor_Hal(uint8_t _red, uint8_t _green, uint8_t _blue);

    void SetColor(uint32_t color);
    void SetColor_Hal(uint32_t color);

    void SetColor();
    void SetColor_Hal();

    void update();

private:
    uint8_t res = 0;

    uint8_t txbuf[24]{};

    SuperSPI spiPlus;

    void load();

    void load_hal();

    static void delay(uint32_t time) {
        osDelay(time);
    }

    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
inline void RGBLED::SetColor() {
    load();
}
inline void RGBLED::SetColor_Hal() {
    load_hal();
}

inline void RGBLED::update() {
    SetColor();
    red++;
    green += 5;
    blue += 10;
    delay(1);
    red++;
    green++;
    blue++;
    delay(100);
}

extern RGBLED Led;

#endif //DM_42_RGBLED_H
