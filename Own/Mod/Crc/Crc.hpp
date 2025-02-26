#pragma once

/* 头文件引用 ----------------------------------------------------------------------------------------------------------*/
#include "cstdint"

/* 函数声明 ------------------------------------------------------------------------------------------------------------*/
namespace crc {
    uint8_t get_crc8_check_sum(uint8_t *pchMessage, uint32_t dwLength, uint8_t ucCRC8);

    bool verify_crc8_check_sum(uint8_t *pchMessage, uint32_t dwLength);

    void append_crc8_check_sum(uint8_t *pchMessage, uint32_t dwLength);

    uint16_t get_crc16_check_sum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);

    bool verify_crc16_check_sum(uint8_t *pchMessage, uint32_t dwLength);

    void append_crc16_check_sum(uint8_t *pchMessage, uint32_t dwLength);
}
