#pragma once

#include <fstream>

class Point
{
    public:
        double x;
        double y;
        Point(double x, double y);
        Point();
        friend bool operator== (const Point& lhs, const Point& rhs);
        bool operator< (const Point& rhs);
        bool operator> (const Point& rhs);
        friend std::ostream& operator<< (std::ostream& os, const Point& point);
        friend double distance(const Point& point1, const Point& point2);
};