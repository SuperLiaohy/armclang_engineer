//
// Created by liaohy on 25-3-18.
//

#pragma once

#include "CDC/SuperCDC.hpp"

class PC : public SuperCDC{
public:
    PC(uint8_t head, uint8_t tail) : head(head), tail(tail) {}

    interact_dep::receive_data_t receive_data{};
    interact_dep::transmit_data_t transmit_data{};

    uint8_t head;
    uint8_t tail;
};
