#include "Utils.h"
#include <cmath>
#include <numbers>

double utils::convertAzimuthToAngle(double azimuth)
{
    if (azimuth >= 0 && azimuth <= 90)
        azimuth = 90 - azimuth;
    else if (azimuth > 90 && azimuth <= 270)
        azimuth = 90 - azimuth + 180;
    else if (azimuth > 270 && azimuth <= 360)
        azimuth = 270 - azimuth + 180;
    return azimuth;
}

double utils::convertDegreesToRadians(double angle)
{
    return angle * std::numbers::pi / 180.0;
}