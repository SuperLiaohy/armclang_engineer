//
// Created by liaohy on 25-3-18.
//

#pragma once

#include "CDC/SuperCDC.hpp"

class PC : public SuperCDC{
public:
    interact_dep::receive_data_t receive_data{};
    interact_dep::transmit_data_t transmit_data{};
};
