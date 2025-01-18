//
// Created by Administrator on 24-10-1.
//

#include "RGBLED.h"

#define WS2812_LowLevel    0xC0     // 0码
#define WS2812_HighLevel   0xF0     // 1码


void RGBLED::load() {
    for (int i = 0; i < 8; i++) {
        txbuf[7 - i] = (((green >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
        txbuf[15 - i] = (((red >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
        txbuf[23 - i] = (((blue >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
    }
    spiPlus.WriteDMA(&res, 0);
    osDelay(1);
    spiPlus.WriteDMA(txbuf, 24);
    for (int i = 0; i < 100; ++i) {
        spiPlus.WriteDMA(&res, 1);
    }
}

void RGBLED::load_hal() {
    for (int i = 0; i < 8; i++) {
        txbuf[7 - i] = (((green >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
        txbuf[15 - i] = (((red >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
        txbuf[23 - i] = (((blue >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
    }
    spiPlus.WriteDMA(&res, 0);
    HAL_Delay(1);
    spiPlus.WriteDMA(txbuf, 24);
    for (int i = 0; i < 100; ++i) {
        spiPlus.WriteDMA(&res, 1);
    }
}

void RGBLED::SetColor(uint32_t color) {
    red = (color >> 16) & 0xff;
    green = (color >> 8) & 0xff;
    blue = color & 0xff;
    load();
}
void RGBLED::SetColor_Hal(uint32_t color) {
    red = (color >> 16) & 0xff;
    green = (color >> 8) & 0xff;
    blue = color & 0xff;
    load_hal();
}

void RGBLED::SetColor(uint8_t _red, uint8_t _green, uint8_t _blue) {
    red = _red;
    green = _green;
    blue = _blue;
    load();
}
void RGBLED::SetColor_Hal(uint8_t _red, uint8_t _green, uint8_t _blue) {
    red = _red;
    green = _green;
    blue = _blue;
    load_hal();
}

