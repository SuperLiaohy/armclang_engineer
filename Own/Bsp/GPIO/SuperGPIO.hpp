//
// Created by liaohy on 8/30/24.
//
#pragma once

#include "concepts"

#ifdef __cplusplus
extern "C" {
#endif
#include "gpio.h"
#ifdef __cplusplus
}
#endif

template<typename T>
concept is_io = requires(T t, int32_t value) {
    typename T::state;
    requires requires {
        t.WriteUp();
        t.WriteDown();
        t.Write(value);
        t.Toggle();
    } || requires {
        { t.Read() } -> std::same_as<typename T::state>;
    };
};

enum pin_mode {
    INPUT = 0,
    OUTPUT,
    ALTERNATE,
    OPEN_DRAIN,
};

constexpr uint32_t GPIO_BASE = GPIOA_BASE;

//constexpr uint32_t gpio_map_offset(uint32_t gpio) {
//    return gpio - GPIO_BASE;
//}

constexpr uint32_t gpio_map(uint32_t gpio) {
    return GPIO_BASE + (gpio - GPIO_BASE) * sizeof(GPIO_TypeDef);
}

template<uint32_t gpio, pin_mode mode, uint16_t pin>
class SuperGPIO {
public:
    using state = GPIO_PinState;
public:
    SuperGPIO() = default;

    ~SuperGPIO() = default;

    inline void WriteUp()
        requires(mode == OUTPUT || mode == OPEN_DRAIN);

    inline void WriteDown()
        requires(mode == OUTPUT || mode == OPEN_DRAIN);

    void Write(int32_t value)
        requires(mode == OUTPUT || mode == OPEN_DRAIN);

    void Toggle()
        requires(mode == OUTPUT || mode == OPEN_DRAIN);

    state Read()
        requires(mode == INPUT);

    friend void ::HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
};

template<uint32_t gpio, pin_mode mode, uint16_t pin>
inline void SuperGPIO<gpio, mode, pin>::WriteUp()
    requires(mode == OUTPUT || mode == OPEN_DRAIN)
{
    HAL_GPIO_WritePin(
        reinterpret_cast<GPIO_TypeDef*>(gpio), pin, GPIO_PIN_SET);
}

template<uint32_t gpio, pin_mode mode, uint16_t pin>
inline void SuperGPIO<gpio, mode, pin>::WriteDown()
    requires(mode == OUTPUT || mode == OPEN_DRAIN)
{
    HAL_GPIO_WritePin(
        reinterpret_cast<GPIO_TypeDef*>(gpio), pin, GPIO_PIN_RESET);
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
        reinterpret_cast<GPIO_TypeDef*>(gpio), pin);
}

template<uint32_t gpio, pin_mode mode, uint16_t pin>
SuperGPIO<gpio, mode, pin>::state SuperGPIO<gpio, mode, pin>::Read()
    requires(mode == INPUT)
{
    return HAL_GPIO_ReadPin(
        reinterpret_cast<GPIO_TypeDef*>(gpio), pin);
}

extern SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_15> power_5v;
extern SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_14> power_24v_right;
extern SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_13> power_24v_left;

