#include "sprite.h"

#include <utility>

Sprite::Sprite(Point2D<float> position, size_t textureID) :
    mPosition(std::move(position)),
    mTextureID(textureID)
{
}

float Sprite::getX() const
{
    return mPosition.x;
}

void Sprite::setX(float value)
{
    mPosition.x = value;
}

float Sprite::getY() const
{
    return mPosition.y;
}

void Sprite::setY(float value)
{
    mPosition.y = value;
}

size_t Sprite::getID() const
{
    return mTextureID;
}