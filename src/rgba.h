#pragma once

#include <cstdint>

struct RGBA
{
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    static RGBA white() { return RGBA{}; };
    static RGBA black() { return RGBA{0, 0, 0, 255}; };
};