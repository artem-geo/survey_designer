#include "Rectangle.h"
#include "shapefil.h"
#include "ShpInfo.h"
#include "Utils.h"
#include <iostream>

int main()
{
    double azimuthGrad = 280;
    double dL = 100.0;
    double angleGrad = utils::convertAzimuthToAngle(azimuthGrad);
    std::cout << angleGrad << std::endl;

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
    
    Line line1(Point(0, 0), Point(5, 5));
    Line line2(Point(3, 0), Point(3, 7));

    std::cout << std::boolalpha;
    std::cout << Line::checkSegmentsIntersection(line1, line2) << std::endl;
    std::cout << Line::getIntersection(line1, line2) << std::endl;

    SHPDestroyObject(polygon);
    SHPClose(inHandle);

    // std::cin.get();
}