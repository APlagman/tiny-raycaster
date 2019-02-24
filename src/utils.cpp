#include "utils.h"

#include "framebuffer.h"
#include "rgba.h"

#include <cassert>
#include <fstream>
#include <iostream>

uint32_t packColor(const RGBA& rgba)
{
    return (rgba.a << 24) + (rgba.b << 16) + (rgba.g << 8) + rgba.r;
}

RGBA unpackColor(uint32_t color)
{
    RGBA result;
    result.r = (color >> 0) & 255;
    result.g = (color >> 8) & 255;
    result.b = (color >> 16) & 255;
    result.a = (color >> 24) & 255;
    return result;
}

void writePPMImage(const std::string& filename, const FrameBuffer& image)
{
    std::ofstream ofs(filename, std::ofstream::out | std::ofstream::binary);
    ofs << "P6\n" << image.getWidth() << " " << image.getHeight() << "\n255\n";
    for (const auto& pixel : image) {
        const RGBA color = unpackColor(pixel);
        ofs << static_cast<char>(color.r);
        ofs << static_cast<char>(color.g);
        ofs << static_cast<char>(color.b);
    }
    ofs.close();
}