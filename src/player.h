#pragma once

#include "direction.h"
#include "point2D.h"

class Player
{
public:
    Player(Point2D<float> position, float angle, float fieldOfView);

    float getX() const;
    void setX(float value);
    float getY() const;
    void setY(float value);
    float getAngle() const;
    void setAngle(float value);
    float getFieldOfView() const;
    Direction getWalkingDirection() const;
    void setWalkingDirection(Direction value);
    Direction getTurningDirection() const;
    void setTurningDirection(Direction value);

private:
    Point2D<float> mPosition;
    float mAngle;
    float mFieldOfView;
    Direction mTurning = Direction::None;
    Direction mWalking = Direction::None;
};