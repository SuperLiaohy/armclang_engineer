//
// Created by lhy on 2024/9/9.
//
#pragma once

#include "../Motor.tpp"
#define USING_M3508 1
#if USING_M3508 == 1


class M3508 : public default_motor<8192, 3591, 187> {
public:
    explicit M3508(const uint16_t rx_id)
        : default_motor(rx_id) {};

    static constexpr FOC foc = {0x200, 0x200, 0x1FF};;
};



#endif //USING_M3508
