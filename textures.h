#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "dimension2D.h"
#include "point2D.h"

class TextureSet
{
public:
    TextureSet(const std::string& filename);

    size_t getCount() const;
    size_t getTextureWidth() const;
    size_t getTextureHeight() const;
    uint32_t get(const Point2D<size_t>& point, size_t id) const;
    std::vector<uint32_t> getScaledColumn(size_t id, size_t xCoordinate, size_t height) const;

private:
    Dimension2D<size_t> mImageSize;
    size_t mTextureCount;
    Dimension2D<size_t> mTextureSize;
    std::vector<uint32_t> mImage;
};