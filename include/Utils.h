#pragma once

#include "Point.h"

namespace utils
{
    const Point POINT_DUMMY = Point(-999, -999);
    double convertAzimuthToAngle(double azimuth);
    double convertDegreesToRadians(double angle);
}