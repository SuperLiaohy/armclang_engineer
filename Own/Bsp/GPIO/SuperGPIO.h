//
// Created by liaohy on 8/30/24.
//

#ifndef NONE_PRJ_SUPERGPIO_H
#define NONE_PRJ_SUPERGPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

#ifdef __cplusplus
}
#endif

enum mode {
    INPUT = 0,
    OUTPUT,
    ALTERNATE,
    ANALOG,
    OPEN_DRAIN
};

class SuperGPIO {
private:
    GPIO_TypeDef *gpio;
    uint16_t pin;
    mode mod;
public:
    SuperGPIO(GPIO_TypeDef *_gpio, uint16_t _pin, mode _mod);

    ~SuperGPIO() = default;

    void WriteUp();

    void WriteDown();

    void Write(int32_t value);

    void Toggle();

    GPIO_PinState Read();

    friend void::HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

};

inline void SuperGPIO::WriteUp() {
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
}

inline void SuperGPIO::WriteDown() {
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}

inline void SuperGPIO::Write(int32_t value) {
    value ? WriteUp() : WriteDown();
}

inline void SuperGPIO::Toggle() {
    HAL_GPIO_TogglePin(gpio, pin);
}

inline GPIO_PinState SuperGPIO::Read() {
    return HAL_GPIO_ReadPin(gpio, pin);
}
#endif //NONE_PRJ_SUPERGPIO_H
