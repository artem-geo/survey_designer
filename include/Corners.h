#pragma once

#include "point.h"


struct Corners {
    Point bottom_left;
    Point top_left;
    Point top_right;
    Point bottom_right;

    Corners() = default;
    ~Corners() = default;
};