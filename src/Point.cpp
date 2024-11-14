#include "Point.h"
#include <cmath>


Point::Point(double x, double y)
    : x(x), y(y) {}

Point::Point()
    : Point(-1.0, -1.0) {}

bool operator== (const Point& lhs, const Point& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool Point::operator< (const Point& rhs)
{
    if (y == rhs.y)
        return x < rhs.x;
    return y < rhs.y;
}

bool Point::operator> (const Point& rhs)
{
    if (y == rhs.y)
        return x > rhs.x;
    return y > rhs.y;
}

std::ostream& operator << (std::ostream& os, const Point& point)
{
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

double distance(const Point& point1, const Point& point2)
{
    return std::sqrt(std::pow((point2.x - point1.x), 2) + std::pow((point2.y - point1.x), 2));
}