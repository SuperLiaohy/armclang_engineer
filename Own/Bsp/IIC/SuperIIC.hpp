//
// Created by Administrator on 25-4-11.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "i2c.h"
#ifdef __cplusplus
}
#endif


class SuperIIC {
public:
    explicit SuperIIC(I2C_HandleTypeDef* hi2c)
        : hi2c(hi2c) {};

    void master_block_transmit(uint16_t dev_address, uint8_t* data, uint16_t size, uint32_t timeout){
        HAL_I2C_Master_Transmit(hi2c, dev_address, data, size, timeout);
    };
    void master_block_receive(uint16_t dev_address, uint8_t* data, uint16_t size, uint32_t timeout){
        HAL_I2C_Master_Receive(hi2c, dev_address, data, size, timeout);
    };

    void slave_block_transmit(uint8_t* data, uint16_t size, uint32_t timeout){
        HAL_I2C_Slave_Transmit(hi2c,  data, size, timeout);
    };
    void slave_block_receive(uint16_t dev_address, uint8_t* data, uint16_t size, uint32_t timeout) {
        HAL_I2C_Slave_Receive(hi2c, data, size, timeout);
    };
    void mem_block_write_bytes(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size,uint8_t* data, uint16_t size, uint32_t timeout) {
        HAL_I2C_Mem_Write(hi2c, dev_address, mem_address, mem_addr_size, data, size, timeout);
    };
    void mem_block_read_bytes(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t* data, uint16_t size, uint32_t timeout) {

        HAL_I2C_Mem_Read(hi2c, dev_address, mem_address, mem_addr_size, data, size, timeout);
    };

    void master_it_transmit(uint16_t dev_address, uint8_t* data, uint16_t size) {
        HAL_I2C_Master_Transmit_IT(hi2c, dev_address, data, size);
    };
    void master_it_receive(uint16_t dev_address, uint8_t* data, uint16_t size){
        HAL_I2C_Master_Receive_IT(hi2c, dev_address, data, size);
    };
    void slave_it_transmit(uint8_t* data, uint16_t size){
        HAL_I2C_Slave_Transmit_IT(hi2c, data, size);
    };
    void slave_it_receive(uint8_t* data, uint16_t size) {
        HAL_I2C_Slave_Receive_IT(hi2c, data, size);
    };
    void mem_it_write_bytes(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t* data, uint16_t size){
        HAL_I2C_Mem_Write_IT(hi2c, dev_address, mem_address, mem_addr_size, data, size);
    };
    void mem_it_read_bytes(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t* data, uint16_t size){
        HAL_I2C_Mem_Read_IT(hi2c, dev_address, mem_address, mem_addr_size, data, size);
    };

    void master_dma_transmit(uint16_t dev_address, uint8_t* data, uint16_t size) {
        HAL_I2C_Master_Transmit_DMA(hi2c, dev_address, data, size);
    };
    void master_dma_receive(uint16_t dev_address, uint8_t* data, uint16_t size){
        HAL_I2C_Master_Receive_DMA(hi2c, dev_address, data, size);
    };
    void slave_dma_transmit(uint16_t dev_address, uint8_t* data, uint16_t size) {
        HAL_I2C_Slave_Transmit_DMA(hi2c, data, size);
    };
    void slave_dma_receive(uint16_t dev_address, uint8_t* data, uint16_t size) {
        HAL_I2C_Slave_Receive_DMA(hi2c, data, size);
    };
    void mem_dma_write_bytes(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t* data, uint16_t size) {
        HAL_I2C_Mem_Write_DMA(hi2c, dev_address, mem_address, mem_addr_size, data, size);
    };
    void mem_dma_read_bytes(uint16_t dev_address, uint16_t mem_address, uint16_t mem_addr_size, uint8_t* data, uint16_t size) {
        HAL_I2C_Mem_Read_DMA(hi2c, dev_address, mem_address, mem_addr_size, data, size);
    };
private:
    I2C_HandleTypeDef* hi2c;
};
