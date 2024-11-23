#pragma once

#include "Point.h"


struct Corners {
    Point bottom_left;
    Point top_left;
    Point top_right;
    Point bottom_right;

    Corners() = default;
    ~Corners() = default;
};