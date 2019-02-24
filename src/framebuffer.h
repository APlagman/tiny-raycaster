#pragma once

#include "dimension2D.h"
#include "point2D.h"

#include <cstdint>
#include <vector>

class FrameBuffer
{
public:
    FrameBuffer(Dimension2D<size_t> dimensions);

    size_t getWidth() const;
    size_t getHeight() const;
    void clear(uint32_t color);
    void setPixel(const Point2D<size_t>& pixel, uint32_t color);
    void drawRectangle(const Point2D<size_t>& topLeft,
                       const Dimension2D<size_t>& size, uint32_t color);

    std::vector<uint32_t>::const_iterator begin() const;
    std::vector<uint32_t>::const_iterator end() const;

private:
    Dimension2D<size_t> mSize;
    std::vector<uint32_t> mImage;
};