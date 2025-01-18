//
// Created by liaohy on 8/30/24.
//

#include "SuperGPIO.h"


SuperGPIO::SuperGPIO(GPIO_TypeDef *_gpio, uint16_t _pin, mode _mod) {
    this->gpio = _gpio;
    this->pin = _pin;
    this->mod = _mod;
}
