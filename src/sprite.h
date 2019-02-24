#pragma once

#include "point2D.h"

#include <cstddef>

class Sprite
{
public:
    Sprite(Point2D<float> position, size_t textureID);

    float getX() const;
    void setX(float value);
    float getY() const;
    void setY(float value);

private:
    Point2D<float> mPosition;
    size_t mTextureID;
};