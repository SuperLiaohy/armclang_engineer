//
// Created by liaohy on 8/30/24.
//

#ifndef NONE_PRJ_SUPERGPIO_H
#define NONE_PRJ_SUPERGPIO_H

#include "concepts"

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

#ifdef __cplusplus
}
#endif

enum pin_mode {
    INPUT = 0,
    OUTPUT,
    ALTERNATE,
    OPEN_DRAIN,
};

constexpr uint32_t GPIO_BASE = GPIOA_BASE;

constexpr uint32_t gpio_map_offset(uint32_t gpio) {
    return gpio - GPIO_BASE;
}

constexpr uint32_t gpio_map(uint32_t gpio) {
    return GPIO_BASE + (gpio-GPIO_BASE) * sizeof(GPIO_TypeDef);
}

template<uint32_t gpio, pin_mode mode, uint16_t pin>
class SuperGPIO {
public:
    SuperGPIO()  = default;
    ~SuperGPIO() = default;

    inline void WriteUp()
        requires(mode == OUTPUT || mode == OPEN_DRAIN);

    inline void WriteDown()
        requires(mode == OUTPUT || mode == OPEN_DRAIN);

    void Write(int32_t value)
        requires(mode == OUTPUT || mode == OPEN_DRAIN);

    void Toggle()
        requires(mode == OUTPUT || mode == OPEN_DRAIN);

    GPIO_PinState Read()
        requires(mode == INPUT);

    friend void ::HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
};

template<uint32_t gpio, pin_mode mode, uint16_t pin>
inline void SuperGPIO<gpio, mode, pin>::WriteUp()
    requires(mode == OUTPUT || mode == OPEN_DRAIN)
{
    HAL_GPIO_WritePin(
        reinterpret_cast<GPIO_TypeDef*>(GPIO_BASE) + gpio_map_offset(gpio), pin, GPIO_PIN_SET);
}

template<uint32_t gpio, pin_mode mode, uint16_t pin>
inline void SuperGPIO<gpio, mode, pin>::WriteDown()
    requires(mode == OUTPUT || mode == OPEN_DRAIN)
{
    HAL_GPIO_WritePin(
        reinterpret_cast<GPIO_TypeDef*>(GPIO_BASE) + gpio_map_offset(gpio), pin, GPIO_PIN_RESET);
}

template<uint32_t gpio, pin_mode mode, uint16_t pin>
void SuperGPIO<gpio, mode, pin>::Write(int32_t value)
    requires(mode == OUTPUT || mode == OPEN_DRAIN)
{
    value ? WriteUp() : WriteDown();
}

template<uint32_t gpio, pin_mode mode, uint16_t pin>
void SuperGPIO<gpio, mode, pin>::Toggle()
    requires(mode == OUTPUT || mode == OPEN_DRAIN)
{
    HAL_GPIO_TogglePin(
        reinterpret_cast<GPIO_TypeDef*>(GPIO_BASE) + gpio_map_offset(gpio), pin);
}

template<uint32_t gpio, pin_mode mode, uint16_t pin>
GPIO_PinState SuperGPIO<gpio, mode, pin>::Read()
    requires(mode == INPUT)
{
    return HAL_GPIO_ReadPin(
        reinterpret_cast<GPIO_TypeDef*>(GPIO_BASE) + gpio_map_offset(gpio), pin);
}

#endif //NONE_PRJ_SUPERGPIO_H
