#ifndef CORNERS_H
#define CORNERS_H

#include "Point.h"


struct Corners
{
    Point bottomLeft;
    Point topLeft;
    Point topRight;
    Point bottomRight;

    Corners() = default;
    ~Corners() = default;
};


#endif