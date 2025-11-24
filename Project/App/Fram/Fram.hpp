//
// Created by Administrator on 25-4-11.
//

#pragma once

#include "IIC/SuperIIC.hpp"

class Fram {
public:
    explicit Fram(I2C_HandleTypeDef* hi2c)
        : iicPlus(hi2c) {};
    void block_write(uint16_t mem_address, uint8_t* data, uint16_t size, uint16_t timeout) {
        iicPlus.mem_block_write_bytes(0xA0 | ((mem_address >> 8) << 1), (mem_address & 0x00FF), I2C_MEMADD_SIZE_8BIT,
                                      data, size, timeout);
    };

    void block_read(uint16_t mem_address, uint8_t* data, uint16_t size, uint16_t timeout) {
        iicPlus.mem_block_read_bytes(0xA0 | ((mem_address >> 8) << 1), (mem_address & 0x00FF), I2C_MEMADD_SIZE_8BIT,
                                     data, size, timeout);
    };

    void it_write(uint16_t mem_address, uint8_t* data, uint16_t size) {
        iicPlus.mem_it_write_bytes(0xA0 | ((mem_address >> 8) << 1), (mem_address & 0x00FF), I2C_MEMADD_SIZE_8BIT,
                                      data, size);
    };

    void it_read(uint16_t mem_address, uint8_t* data, uint16_t size) {
        iicPlus.mem_it_read_bytes(0xA0 | ((mem_address >> 8) << 1), (mem_address & 0x00FF), I2C_MEMADD_SIZE_8BIT,
                                     data, size);
    };

private:
    SuperIIC iicPlus;
};
