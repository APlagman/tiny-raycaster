#include "textures.h"

#include "rgba.h"
#include "utils.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Winline"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#pragma GCC diagnostic pop

#include <cassert>
#include <iostream>

TextureSet::TextureSet(const std::string& filename) :
    mImageSize{0, 0},
    mTextureCount(0),
    mTextureSize{0, 0}
{
    int numChannels = -1;
    int width = 0;
    int height = 0;

    unsigned char* pixmap =
        stbi_load(filename.c_str(), &width, &height, &numChannels, 0);
    if (!pixmap) {
        std::cerr << "Error: cannot load the textures" << std::endl;
        return;
    }

    if (STBI_rgb_alpha != numChannels) {
        std::cerr << "Error: the texture must be a 32 bit image" << std::endl;
        stbi_image_free(pixmap);
        return;
    }

    mImageSize = {static_cast<size_t>(width), static_cast<size_t>(height)};
    mTextureCount = static_cast<size_t>(width / height);
    const size_t indWidth = static_cast<size_t>(width / mTextureCount);
    mTextureSize = {indWidth, indWidth};
    if (mImageSize.width != mImageSize.height * mTextureCount) {
        std::cerr << "Error: the texture file must contain N square textures "
                     "packed horizontally"
                  << std::endl;
        stbi_image_free(pixmap);
        return;
    }

    mImage = std::vector<uint32_t>(mImageSize.width * mImageSize.height);
    for (size_t row = 0; row < mImageSize.height; ++row) {
        for (size_t col = 0; col < mImageSize.width; ++col) {
            RGBA color;
            const size_t index = col + row * mImageSize.width;
            const size_t offset = (index)*STBI_rgb_alpha;
            color.r = pixmap[offset + 0];
            color.g = pixmap[offset + 1];
            color.b = pixmap[offset + 2];
            color.a = pixmap[offset + 3];
            mImage[index] = packColor(color);
        }
    }
    stbi_image_free(pixmap);
    return;
}

size_t TextureSet::getCount() const
{
    return mTextureCount;
}

size_t TextureSet::getTextureWidth() const
{
    return mTextureSize.width;
}

size_t TextureSet::getTextureHeight() const
{
    return mTextureSize.height;
}

uint32_t TextureSet::get(const Point2D<size_t>& point, const size_t id) const
{
    assert(point.x < mTextureSize.width && point.y < mTextureSize.height
           && id < mTextureCount);
    return mImage[point.x + id * mTextureSize.width
                  + point.y * mImageSize.width];
}

std::vector<uint32_t> TextureSet::getScaledColumn(const size_t id,
                                                  const size_t xCoordinate,
                                                  const size_t height) const
{
    assert(xCoordinate < mImageSize.width && id < mTextureCount);
    std::vector<uint32_t> column(height);
    for (size_t y = 0; y < height; ++y) {
        column[y] = get({xCoordinate, (y * mTextureSize.height) / height}, id);
    }
    return column;
}