#include "map.h"

#include <cassert>

static const char map[] = "0000222222220000"
                          "1              0"
                          "1      11111   0"
                          "1     0        0"
                          "0     0  1110000"
                          "0     3        0"
                          "0   10000      0"
                          "0   3   11100  0"
                          "5   4   0      0"
                          "5   4   1  00000"
                          "0       1      0"
                          "2       1      0"
                          "0       0      0"
                          "0 0000000      0"
                          "0              0"
                          "0002222222200000";

Map::Map() : mSize{16, 16}
{
    assert(sizeof(map) == getWidth() * getHeight() + 1);
}

size_t Map::getWidth() const
{
    return mSize.width;
}

size_t Map::getHeight() const
{
    return mSize.height;
}

int Map::get(const Point2D<size_t>& point) const
{
    assert(point.x < getWidth() && point.y < getHeight());
    return map[point.x + point.y * getWidth()] - '0';
}

bool Map::isEmpty(const Point2D<size_t>& point) const
{
    assert(point.x < getWidth() && point.y < getHeight());
    return map[point.x + point.y * getWidth()] == ' ';
}