#include "Rectangle.h"
#include "shapefil.h"
#include "ShpInfo.h"
#include "Utils.h"
#include <iostream>

int main()
{
    double azimuth_grad = 280;
    double dL = 100.0;
    double angle_grad = utils::convertAzimuthToAngle(azimuth_grad);
    std::cout << angle_grad << std::endl;

    auto handle_in = SHPOpen(R"(C:\Dev\survey_designer\examples\polygon2)", "rb");
    
    ShpInfo shp_info;
    SHPGetInfo(handle_in, &(shp_info.n_entities), &(shp_info.shp_type), shp_info.padf_min_bound, shp_info.padf_max_bound);
    // check the opened SHP: type == 5 (Polygon) and only one polygon
    if (shp_info.shp_type != 5)
    {
        std::cerr << "Please, provide a Polygon SHP file" << std::endl;
        SHPClose(handle_in);
        return 0;
    }
    if (shp_info.n_entities != 1) 
    {
        std::cerr << "Please, provide a SHP file with ONE polygon" << std::endl;
        SHPClose(handle_in);
    }
    
    auto polygon = SHPReadObject(handle_in, 0);

    Rectangle rectangle;
    rectangle.initRectangle(polygon->padfX, polygon->padfY, polygon->nVertices);
    rectangle.initLines(angle_grad, dL);
    
    Line line1(Point(0, 0), Point(5, 5));
    Line line2(Point(3, 0), Point(3, 7));

    SHPDestroyObject(polygon);
    SHPClose(handle_in);

    // std::cin.get();
}