#include <iostream>
#include "libs/shapefil.h"
#include <algorithm>
#include <vector>

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
};

class Line
{
public:
    std::pair<Point, Point> caps;
    Line() = default;
    Line(Point p1, Point p2)
        : caps(p1, p2) {}

};

// class Profile : public Line
// {
// public:
//     std::vector<Point> stations;
// };

struct Rectangle
{   
    std::vector<Line> sides;

    Rectangle() = default;
    Rectangle(double xArray[], double yArray[], int nPoints)
{
        Point bottomLeft{};
        Point topLeft{};
        Point topRight{};
        Point bottomRight{};
        double xMin{-1}, yMin{-1}, xMax{-1}, yMax{-1};

        for (size_t i{0}; i < nPoints; ++i) 
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
        bottomLeft = {xMin, yMin};
        topLeft = {xMin, yMax};
        topRight = {xMax, yMax};
        bottomRight = {xMax, yMin};
        sides.push_back(Line(bottomLeft, topLeft));
        sides.push_back(Line(topLeft, topRight));
        sides.push_back(Line(topRight, bottomRight));
        sides.push_back(Line(bottomRight, bottomLeft));
    }
};

int main()
{
    auto inHandle = SHPOpen(R"(C:\Dev\survey_designer\examples\polygon)", "rb");
    
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

    SHPDestroyObject(polygon);
    SHPClose(inHandle);

    // std::cin.get();
}