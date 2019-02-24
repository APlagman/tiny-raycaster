#pragma once

template<class T>
struct Point2D
{
    T x = 0;
    T y = 0;
};

template<class From, class To>
Point2D<To> cast(const Point2D<From>& point)
{
    return {static_cast<To>(point.x), static_cast<To>(point.y)};
}

template<class T>
Point2D<T> operator+(const Point2D<T>& lhs, const Point2D<T>& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

template<class T>
Point2D<T> operator-(const Point2D<T>& lhs, const Point2D<T>& rhs)
{
    return lhs + (-rhs);
}

template<class T>
Point2D<T> operator-(const Point2D<T>& orig)
{
    return {-orig.x, -orig.y};
}