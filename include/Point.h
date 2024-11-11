#ifndef POINT_H
#define POINT_H

#include <fstream>

class Point
{
public:
    double x;
    double y;
    Point(double x, double y);
    Point();
    bool operator == (const Point& rhs);
    friend std::ostream& operator << (std::ostream& os, const Point& point);
};


#endif