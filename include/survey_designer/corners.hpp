#pragma once

#include "survey_designer/point.hpp"


struct Corners {
    Point bottom_left;
    Point top_left;
    Point top_right;
    Point bottom_right;

    Corners() = default;
    ~Corners() = default;
};