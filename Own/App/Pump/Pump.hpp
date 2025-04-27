//
// Created by Administrator on 24-10-26.
//
#pragma once

#include "GPIO/SuperGPIO.hpp"
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
    Pump(typename io::handle* port, typename io::handle_pin pin) : handle(port, pin) {}
    void open() {
        handle.WriteDown();
        s = state::open;
    };
    void close() {
        handle.WriteUp();
        s = state::close;
    };

    void toggle() {
        s = state (1 - static_cast<uint8_t>(s));
        handle.Toggle();
    };
private:
    io handle;
    state s;
};


extern Pump<SuperGPIO> pump;
