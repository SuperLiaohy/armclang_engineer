#include "CanServos.hpp"

void CanServos::read() const {
    uint8_t data[8] = {0x01, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(id, data);
}
void CanServos::lock() const {
    uint8_t data[8] = {0x11, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(id, data);
}
void CanServos::unlock() const {
    uint8_t data[8] = {0x2f, 0, 0, 0, 0, 0, 0, 0};
    canPlus->transmit_pdata(id, data);
}
void CanServos::test(uint8_t cmd, int16_t pos, int16_t speed, uint8_t a, uint8_t b, uint8_t c) {
    uint8_t data[8] = {cmd,
                       static_cast<uint8_t>((pos & 0xff)), static_cast<uint8_t>((pos >> 8)),
                       static_cast<uint8_t>((speed & 0xff)), static_cast<uint8_t>((speed >> 8)),
                       a, b, c};
    canPlus->transmit_pdata(id, data);
}
