//
// Created by lhy on 2024/9/9.
//
#pragma once


namespace my_math {
    constexpr float pi   = 3.14159265358979323846f;
    constexpr float r2d  = 180.0f / pi;
    constexpr float d2r  = pi / 180.0f;
    constexpr float r2b2 = 65536.f / 2 * pi;
    constexpr float b22r = 2 * pi / 65536.f;
    constexpr float d2b2 = 65536.f / 360;
    constexpr float b22d = 360 / 65536.f;
} // namespace my_math

template<typename T>
T limited(T* value, T min, T max) {
    if (*value > max) {
        *value = max;
    } else if (*value < min) {
        *value = min;
    }
    return *value;
}

template<typename T>
T limited(T& value, T min, T max) {
    if (value > max) {
        value = max;
    } else if (value < min) {
        value = min;
    }
    return value;
}

template<typename T>
T limited(const T& value, T min, T max) {
    if (value > max) {
        return max;
    }
    if (value < min) {
        return min;
    }
    return value;
}

template<typename T>
T whileLimit(T& value, T min, T max) {
    float len = max - min;
    if (value > max) {
        value -= len;
    } else if (*value < min) {
        value += len;
    }
    return value;
}

template<typename T>
T whileLimit(T&& value, T min, T max) {
    float len = max - min;
    if (value > max) {
        return value - len;
    } else if (*value < min) {
        return value + len;
    }
}

template<typename T>
T SmoothLimit(T* value, T target, T smooth) {
    if (*value > target) {
        if (*value - smooth < target) {
            *value = target;
        }
    } else if (*value < target) {
        if (*value + smooth > target) {
            *value = target;
        }
    }
    return *value;
}

template<typename T>
T offset_transmit(T value, T offset, T min, T max) {
    value += offset;
    if (value > max) {
        value = value - max + min;
    } else if (value < min) {
        value = value + max - min;
    }
    return value;
}
//从source转换到head，source_scale是source的最大值，head_scale是head的最大值，最新默认为0
template<typename source, typename head>
head scale_transmit(source value, source source_scale, head head_scale) {
    head target = static_cast<float>(value * head_scale) / source_scale;
    return target;
}
template<typename source, typename head>
consteval head static_scale_transmit(source value, source source_scale, head head_scale) {
    head target = static_cast<float>(value * head_scale) / source_scale;
    return target;
}
template<typename T>
bool isInRange(T value, T min, T max) {
    return value >= min && value <= max;
}
template<typename T>
bool isInRange(T value, T min, T max, T err) {
    return (value >= min - err) && (value <= max + err);
}

template<typename T>
bool isApproxEqual(T a, T b, T err) {
    return (a <= b + err) && (a >= b - err);
}

template<typename T>
T my_abs(T value) {
    return value > 0 ? value : -value;
}

consteval float rad2deg(float rad) {
    return rad * 180 / my_math::pi;
}
consteval float deg2rad(float deg) {
    return deg * my_math::pi / 180;
}
consteval float scale(float src, float head) {
    return head / src;
}

int float_to_uint(float x_float, float x_min, float x_max, int bits);

float uint_to_float(int x_int, float x_min, float x_max, int bits);
