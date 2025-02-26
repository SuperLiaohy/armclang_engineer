//
// Created by Administrator on 24-10-26.
//
#pragma once

#include "GPIO/SuperGPIO.h"
namespace pump_dep {
    enum class state : uint8_t {
        close = 0,
        open  = 1,
    };

}

template<is_io io>
class Pump {
    using state = pump_dep::state;
public:
    void open() {
        pin.WriteDown();
        s = state::open;
    };
    void close() {
        pin.WriteUp();
        s = state::close;
    };

    void toggle() {
        s = state (1 - static_cast<uint8_t>(s));
        pin.Toggle();
    };
private:
    io pin;
    state s;
};


extern Pump<SuperGPIO<GPIOA_BASE,OUTPUT,GPIO_PIN_0>> pump;
