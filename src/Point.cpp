#include "Point.h"


Point::Point(double x, double y)
    : x(x), y(y) {}

Point::Point()
    : Point(-1.0, -1.0) {}

bool Point::operator == (const Point& rhs)
{
    return (x == rhs.x) && (y == rhs.y);
}

std::ostream& operator << (std::ostream& os, const Point& point)
{
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}