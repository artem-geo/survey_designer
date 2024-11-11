#include <iostream>
#include "libs/shapefil.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <vector>

double convertAzimuthToAngle(double azimuth)
{
    if (azimuth >= 0 && azimuth <= 90)
        azimuth = 90 - azimuth;
    else if (azimuth > 90 && azimuth <= 270)
        azimuth = 90 - azimuth + 180;
    else if (azimuth > 270 && azimuth <= 360)
        azimuth = 270 - azimuth + 180;
    return azimuth;
}

double convertDegreesToRadians(double angle)
{
    return angle * std::numbers::pi / 180;
}

class ShpInfo
{
public:
    int numEntities {-1};
    int shpType {-1};
    double padfMinBound[4];
    double padfMaxBound[4];
    ShpInfo() = default;
    void printInfo()
    {
        std::cout << "Number of entities: " << numEntities << "\n";
        std::cout << "SHP type: " << shpType << "\n";
        std::cout << "Padding values: " << "\n";
        for (size_t i{0}; i < 4; ++i)
            std::cout << "Min " << i << ": " << padfMinBound[i] << " Max " << ": " << padfMaxBound[i] << "\n";
    }
};

class Point
{
public:
    double x;
    double y;
    Point(double x, double y)
        : x(x), y(y) {}

    Point()
        : Point(-1.0, -1.0) {}

    bool operator < (const Point& rhs)
    {
        if (y == rhs.y)
        {
            return x < rhs.x;
        }
        return y < rhs.y;
    }

    bool operator > (const Point& rhs)
    {
        if (y == rhs.y)
        {
            return x > rhs.x;
        }
        return y > rhs.y;
    }

    bool operator == (const Point& rhs)
    {
        return (x == rhs.x) && (y == rhs.y);
    }

    friend std::ostream& operator << (std::ostream& os, const Point& point)
    {
        os << "(" << point.x << ", " << point.y << ")";
        return os;
    } 
};

class Line
{
public:
    std::pair<Point, Point> caps;
    Line() = default;
    Line(Point p1, Point p2)
        : caps(p1, p2) {}
    friend std::ostream& operator << (std::ostream& os, const Line& line)
    {
        os << "[" << line.caps.first << ", " << line.caps.second << "]";
        return os;
    }

};

// class Profile : public Line
// {
// public:
//     std::vector<Point> stations;
// };

struct Corners
{
    Point bottomLeft;
    Point topLeft;
    Point topRight;
    Point bottomRight;

    Corners() = default;
    ~Corners() = default;
};

class Rectangle
{   
public:
    Corners corners;
    std::vector<Line> sides;
    std::vector<Line> lines;
    
    void initRectangle(double xArray[], double yArray[], int nVertices)
    {
        Point bottomLeft{};
        Point topLeft{};
        Point topRight{};
        Point bottomRight{};
        double xMin{-1}, yMin{-1}, xMax{-1}, yMax{-1};

        for (size_t i{0}; i < (nVertices-1); ++i) // nVertices = nVertices(actual) + 1
        {
            if (i == 0)
            {
                xMin = xMax = xArray[i];
                yMin = yMax = yArray[i];
                continue;
            }
            xMin = (xMin > xArray[i]) ? xArray[i] : xMin;
            xMax = (xMax < xArray[i]) ? xArray[i] : xMax;
            yMin = (yMin > yArray[i]) ? yArray[i] : yMin;
            yMax = (yMax < yArray[i]) ? yArray[i] : yMax;
        }

        corners.bottomLeft  = {xMin, yMin};
        corners.topLeft = {xMin, yMax};
        corners.topRight = {xMax, yMax};
        corners.bottomRight = {xMax, yMin};

        sides.push_back(Line(corners.bottomLeft, corners.topLeft));
        sides.push_back(Line(corners.topLeft, corners.topRight));
        sides.push_back(Line(corners.topRight, corners.bottomRight));
        sides.push_back(Line(corners.bottomRight, corners.bottomLeft));
    }

    void printRectangle()
    {
        for (size_t i{0}; i < sides.size(); i++)
        {
            std::cout << "Line " << i << ": " << sides.at(i) << "\n";
        }
    }

    void initLines(double angleGrad, double dL)
    {
        Point pointBegin, pointEnd;
        double angleRad = convertDegreesToRadians(angleGrad);
        if (angleGrad == 0)
        {
            pointBegin = corners.bottomLeft;
            pointEnd = corners.bottomRight;
            do
            {
                lines.push_back(Line(pointBegin, pointEnd));
                pointBegin.y += dL;
                pointEnd.y += dL;
            } while (pointBegin.y <= corners.topLeft.y);
        }
        else if (angleGrad == 90)
        {
            pointBegin = corners.bottomLeft;
            pointEnd = corners.topLeft;
            do
            {
                lines.push_back(Line(pointBegin, pointEnd));
                pointBegin.x += dL;
                pointEnd.x += dL;
            } while (pointBegin.x <= corners.bottomRight.x);
        }
        else if (angleGrad < 90)
        {
            // central point of a line
            Point pointCentral = corners.bottomRight;
            
            // delta x and y to calculate line caps from the central point
            double xDeltaCaps = corners.bottomRight.x - corners.bottomLeft.x;
            double yDeltaCaps = xDeltaCaps * std::tan(angleRad);

            // angle between the rectangle's diagonal and the bottom edge
            double betaRad = std::atan((corners.topRight.y - corners.bottomRight.y) / (xDeltaCaps));
            // step of central point along the diagonal
            double deltaDiagonal = dL / std::sin(angleRad + betaRad);

            // delta x and y to calculate new coord-s of the central point
            double xDeltaCentral = deltaDiagonal * std::sin(betaRad);
            double yDeltaCentral = deltaDiagonal * std::cos(betaRad);

            // push line and update central point until central point is within the rectangle
            while ((pointCentral.x > corners.bottomLeft.x) && (pointCentral.y < corners.topLeft.y))
            {
                lines.push_back(Line(Point(pointCentral.x - xDeltaCaps, pointCentral.y - yDeltaCaps), 
                                    Point(pointCentral.x + xDeltaCaps, pointCentral.y + yDeltaCaps)));
                pointCentral.x -= xDeltaCentral;
                pointCentral.y += yDeltaCentral;
            }            
        }
        else if (angleGrad > 90)
        {
            angleRad = std::numbers::pi - angleRad;

            // central point of a line
            Point pointCentral = corners.bottomLeft;
            
            // delta x and y to calculate line caps from the central point
            double xDeltaCaps = corners.bottomRight.x - corners.bottomLeft.x;
            double yDeltaCaps = xDeltaCaps * std::tan(angleRad);

            // angle between the rectangle's diagonal and the bottom edge
            double betaRad = std::atan((corners.topRight.y - corners.bottomRight.y) / (xDeltaCaps));
            // step of central point along the diagonal
            double deltaDiagonal = dL / std::sin(angleRad + betaRad);

            // delta x and y to calculate new coord-s of the central point
            double xDeltaCentral = deltaDiagonal * std::sin(betaRad);
            double yDeltaCentral = deltaDiagonal * std::cos(betaRad);

            // push line and update central point until central point is within the rectangle
            while ((pointCentral.x < corners.bottomRight.x) && (pointCentral.y < corners.topRight.y))
            {
                lines.push_back(Line(Point(pointCentral.x + xDeltaCaps, pointCentral.y - yDeltaCaps), 
                                    Point(pointCentral.x - xDeltaCaps, pointCentral.y + yDeltaCaps)));
                pointCentral.x += xDeltaCentral;
                pointCentral.y += yDeltaCentral;
            }   
        }
    }
};

int main()
{
    double azimuthGrad = 280;
    double dL = 100.0;
    double angleGrad = convertAzimuthToAngle(azimuthGrad);

    auto inHandle = SHPOpen(R"(C:\Dev\survey_designer\examples\polygon2)", "rb");
    
    ShpInfo shpInfo;
    SHPGetInfo(inHandle, &(shpInfo.numEntities), &(shpInfo.shpType), shpInfo.padfMinBound, shpInfo.padfMaxBound);
    // check the opened SHP: type == 5 (Polygon) and only one polygon
    if (shpInfo.shpType != 5)
    {
        std::cerr << "Please, provide a Polygon SHP file" << std::endl;
        SHPClose(inHandle);
        return 0;
    }
    if (shpInfo.numEntities != 1) 
    {
        std::cerr << "Please, provide a SHP file with ONE polygon" << std::endl;
        SHPClose(inHandle);
    }
    
    auto polygon = SHPReadObject(inHandle, 0);

    Rectangle rectangle;
    rectangle.initRectangle(polygon->padfX, polygon->padfY, polygon->nVertices);
    rectangle.initLines(angleGrad, dL);

    SHPDestroyObject(polygon);
    SHPClose(inHandle);

    // std::cin.get();
}