#include "Line.h"

Line::Line(Point p1, Point p2)
        : caps(p1, p2) {}

std::ostream& operator << (std::ostream& os, const Line& line)
{
    os << "[" << line.caps.first << ", " << line.caps.second << "]";
    return os;
}

/**
 * @brief calculates intesection parameters t and u from the intersection formulae (see https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection)
 * @return pair of doubles where pair.first = t and pair.second = u
 */
std::pair<double, double> Line::getSegmentIntersectionParameters(const Line& line1, const Line& line2)
{        
    // to simplify the formulae components are rewritten explicitly
    double x1 = line1.caps.first.x;
    double y1 = line1.caps.first.y;
    double x2 = line1.caps.second.x;
    double y2 = line1.caps.second.y;

    double x3 = line2.caps.first.x;
    double y3 = line2.caps.first.y;
    double x4 = line2.caps.second.x;
    double y4 = line2.caps.second.y;

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / 
                ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / 
                ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    return {t, u};
}
    
/**
 * @brief checks whether two lines intersect
 * @param t first intersection parameter
 * @param u second intersectino parameter
 * @return true if 0 <= t <= 1 and 0 <= u <= 1
 */
bool Line::checkSegmentsIntersection(const Line& line1, const Line& line2)
{
    auto [t, u] = getSegmentIntersectionParameters(line1, line2);
    bool checkIntersect = ((0 <= t) && (t <= 1)) && ((0 <= u) && (u <= 1));
    return checkIntersect;
}
    
/**
 * @brief calculates line intersection
 * @param line1 line object
 * @param line2 line object
 * @return line1 and line2 intersection point; {-999, -999} - if points do not intersect
 */
Point Line::getIntersection(const Line& line1, const Line& line2)
{
    auto [t, u] = getSegmentIntersectionParameters(line1, line2);
    if (checkSegmentsIntersection(line1, line2))
    {
        double xIntersection = line1.caps.first.x + t * (line1.caps.second.x - line1.caps.first.x);
        double yIntersection = line1.caps.first.y + t * (line1.caps.second.y - line1.caps.first.y);
        return Point(xIntersection, yIntersection);            
    }
    return Point(-999, -999); // return dummy point if no intersection is detected
}