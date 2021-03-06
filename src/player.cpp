#include "player.h"

#include <cassert>
#include <utility>

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

Direction Player::getWalkingDirection() const
{
    return mWalking;
}

void Player::setWalkingDirection(Direction value)
{
    mWalking = value;
}

Direction Player::getTurningDirection() const
{
    return mTurning;
}

void Player::setTurningDirection(Direction value)
{
    mTurning = value;
}