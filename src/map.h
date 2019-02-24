#pragma once

#include "dimension2D.h"
#include "point2D.h"

#include <cstddef>

class Map
{
public:
    Map();

    size_t getWidth() const;
    size_t getHeight() const;
    int get(const Point2D<size_t>& point) const;
    bool isEmpty(const Point2D<size_t>& point) const;

private:
    Dimension2D<size_t> mSize;
};