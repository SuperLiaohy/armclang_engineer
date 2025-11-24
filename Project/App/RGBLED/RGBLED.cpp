//
// Created by Administrator on 24-10-1.
//

#include "RGBLED.hpp"

#define WS2812_LowLevel 0xC0  // 0码
#define WS2812_HighLevel 0xF0 // 1码


void RGBLED::load() {
    for (int i = 0; i < 8; i++) {
        txBuf[7 - i]  = (((green >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
        txBuf[15 - i] = (((red >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
        txBuf[23 - i] = (((blue >> i) & 0x01) ? WS2812_HighLevel : WS2812_LowLevel) >> 1;
    }
    spiPlus.transmit_dma(txBuf, 124);
}

void RGBLED::SetColor(uint32_t color) {
    red   = (color >> 16) & 0xff;
    green = (color >> 8) & 0xff;
    blue  = color & 0xff;
    load();
}

void RGBLED::SetColor(uint8_t _red, uint8_t _green, uint8_t _blue) {
    red   = _red;
    green = _green;
    blue  = _blue;
    load();
}

