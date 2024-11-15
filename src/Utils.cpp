#include "Utils.h"
#include <cmath>
#include <numbers>

/**
 * @brief Converts azimuth deg to angle deg
 * @param azimuth azimuth angle [0, 360] deg
 * @return [0, 180) deg where 0 deg - East, 90 deg - North
 */
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

/**
 * @brief converts angle in degree to angle in radians
 * @param angle angle (degree)
 * @return angle (radians)
 */
double utils::convertDegreesToRadians(double angle)
{
    return angle * std::numbers::pi / 180.0;
}