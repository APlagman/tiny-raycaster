#include <cassert>
#include <utility>

#include "player.h"

Player::Player(Point2D<float> position, float angle, float fieldOfView) :
    mPosition(std::move(position)),
    mAngle(angle),
    mFieldOfView(fieldOfView)
{
}

float Player::getX() const
{
    return mPosition.x;
}

void Player::setX(float value)
{
    mPosition.x = value;
}

float Player::getY() const
{
    return mPosition.y;
}

void Player::setY(float value)
{
    mPosition.y = value;
}

float Player::getAngle() const
{
    return mAngle;
}

void Player::setAngle(float value)
{
    mAngle = value;
}

float Player::getFieldOfView() const
{
    return mFieldOfView;
}