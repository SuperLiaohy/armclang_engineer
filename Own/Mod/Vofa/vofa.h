//
// Created by Administrator on 24-12-6.
//
#pragma once

#include "cstdint"

template <uint8_t n>
class Vofa {
public:
    Vofa()= default;
    float data[n+1]{};
    uint8_t length{};

    template <typename... Args>
    uint8_t* write(Args... args) {
        static_assert(sizeof...(Args) <= n, "Number of arguments exceeds the size of the array");
        float load[] = {static_cast<float>(args)...};
        length = sizeof...(Args) * 4 + 4;
        for (std::size_t i = 0; i < sizeof...(Args); ++i) {
            data[i] = load[i];
        }
        data[sizeof...(Args)] = *reinterpret_cast<const float *>(tail);
        return reinterpret_cast<uint8_t*>(data);
    }
private:
    constexpr static uint8_t tail[4]{0x00, 0x00, 0x80, 0x7f};

};


