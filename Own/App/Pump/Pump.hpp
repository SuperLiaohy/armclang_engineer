//
// Created by Administrator on 24-10-26.
//
#pragma once

#include "GPIO/SuperGPIO.hpp"

template<is_io io>
class Pump {
    enum class state : uint8_t {
        close = 0,
        open  = 1,
    };

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
