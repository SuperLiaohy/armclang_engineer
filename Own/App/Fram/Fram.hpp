//
// Created by Administrator on 25-4-11.
//

#pragma once

#include "IIC/SuperIIC.hpp"

class Fram {
public:
    explicit Fram(I2C_HandleTypeDef* hi2c)
        : iicPlus(hi2c) {};

    void write(uint16_t mem_address, uint8_t* data, uint16_t size, uint16_t timeout) {
        iicPlus.mem_block_write_bytes(0b1010000 | ((mem_address >> 8) & 0x07), (mem_address & 0xFF),
                                      I2C_MEMADD_SIZE_8BIT, data, size, timeout);
    };

    void read(uint16_t mem_address, uint8_t* data, uint16_t size, uint16_t timeout) {
        iicPlus.mem_block_read_bytes(0b1010000 | ((mem_address >> 8) & 0x07), (mem_address & 0xFF),
                                     I2C_MEMADD_SIZE_8BIT, data, size, timeout);
    };

private:
    SuperIIC iicPlus;
};
