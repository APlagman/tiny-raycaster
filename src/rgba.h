#pragma once

#include <cstdint>

struct RGBA
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    static RGBA white() { return RGBA{}; };
};