#pragma once

#include "Point.h"
#include <fstream>

class Line
{
    public:
        std::pair<Point, Point> caps;
        Line() = default;
        Line(Point p1, Point p2);
        friend std::ostream& operator << (std::ostream& os, const Line& line);
        static std::pair<double, double> getSegmentIntersectionParameters(const Line& line1, const Line& line2);
        static bool checkSegmentsIntersection(const Line& line1, const Line& line2);
        static Point getIntersection(const Line& line1, const Line& line2);
};