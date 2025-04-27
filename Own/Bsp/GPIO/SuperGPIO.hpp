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
    typename T::handle;
    typename T::handle_pin;
    requires requires {
        t.WriteUp();
        t.WriteDown();
        t.Write(value);
        t.Toggle();
    } || requires {
        { t.Read() } -> std::same_as<typename T::state>;
    };
};

class SuperGPIO {
public:
    using state      = GPIO_PinState;
    using handle     = GPIO_TypeDef;
    using handle_pin = uint16_t;
public:
    SuperGPIO(handle* port, handle_pin pin) : port(port), pin(pin) {};
    SuperGPIO() = delete;
    ~SuperGPIO() = default;

    inline void WriteUp();
    inline void WriteDown();
    inline void Write(int32_t value);
    inline void Toggle();
    inline state Read();

    friend void ::HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

private:
    handle* port;
    handle_pin pin;
};

inline void SuperGPIO::WriteUp() { HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET); }

inline void SuperGPIO::WriteDown() { HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); }

void SuperGPIO::Write(int32_t value) { value ? WriteUp() : WriteDown(); }

void SuperGPIO::Toggle() { HAL_GPIO_TogglePin(port, pin); }

SuperGPIO::state SuperGPIO::Read() { return HAL_GPIO_ReadPin(port, pin); }

// extern SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_15> power_5v;
// extern SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_14> power_24v_right;
// extern SuperGPIO<GPIOC_BASE, OUTPUT, GPIO_PIN_13> power_24v_left;

extern SuperGPIO power_5v;
extern SuperGPIO power_24v_right;
extern SuperGPIO power_24v_left;
