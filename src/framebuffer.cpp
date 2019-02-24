#include "framebuffer.h"

#include <cassert>

FrameBuffer::FrameBuffer(Dimension2D<size_t> dimensions) :
    mSize(std::move(dimensions)),
    mImage(mSize.width * mSize.height, 0)
{
}

size_t FrameBuffer::getWidth() const
{
    return mSize.width;
}

size_t FrameBuffer::getHeight() const
{
    return mSize.height;
}

void FrameBuffer::clear(uint32_t color)
{
    for (auto& pixel : mImage) {
        pixel = color;
    }
}

void FrameBuffer::setPixel(const Point2D<size_t>& pixel, uint32_t color)
{
    assert(pixel.x < getWidth() && pixel.y < getHeight());
    mImage[pixel.x + pixel.y * getWidth()] = color;
}

void FrameBuffer::drawRectangle(const Point2D<size_t>& topLeft,
                                const Dimension2D<size_t>& size, uint32_t color)
{
    for (size_t x = 0; x < size.width; ++x) {
        for (size_t y = 0; y < size.height; ++y) {
            Point2D<size_t> pixel = {topLeft.x + x, topLeft.y + y};
            if (pixel.x < getWidth() && pixel.y < getHeight()) {
                setPixel(pixel, color);
            }
        }
    }
}

std::vector<uint32_t>::const_iterator FrameBuffer::begin() const
{
    return mImage.begin();
}

std::vector<uint32_t>::const_iterator FrameBuffer::end() const
{
}