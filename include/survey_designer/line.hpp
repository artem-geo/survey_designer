#pragma once

#include "survey_designer/point.hpp"
#include <fstream>

class Line {
public:
    std::pair<Point, Point> caps;
    Line() = default;
    Line(Point p1, Point p2);
    friend std::ostream& operator<< (std::ostream& os, const Line& line);
    static std::pair<double, double> get_segment_intersection_param(const Line& line1, const Line& line2);
    static bool check_segment_intersection(const Line& line1, const Line& line2);
    static Point get_intersection(const Line& line1, const Line& line2);
};