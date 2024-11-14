#include "shapefil.h"
#include "ShpInfo.h"
#include "SurveyScheme.h"
#include <algorithm>
#include <iostream>

SurveyScheme::SurveyScheme(const char* file_path)
    : rectangle(Rectangle()), angle_grad(-1), angle_rad(-1)
{
    try 
    {
        auto handle_poly = SHPOpen(file_path, "rb");
        ShpInfo shp_info;
        SHPGetInfo(handle_poly, &(shp_info.n_entities), &(shp_info.shp_type), shp_info.padf_min_bound, shp_info.padf_max_bound);

        // check the opened SHP: type == 5 (Polygon) and only one polygon
        if (shp_info.shp_type != 5)
            throw std::ios::failure("Please, provide a Polygon SHP file");
        if (shp_info.n_entities != 1)
            throw std::ios::failure("Please, provide a SHP file with ONE polygon");

        auto polygon = SHPReadObject(handle_poly, 0);
        rectangle.initRectangle(polygon->padfX, polygon->padfY, polygon->nVertices);
        
        for (size_t i{0}; i < ((polygon->nVertices) -1); ++i) 
        {
            Point point1{polygon->padfX[i], polygon->padfY[i]};
            Point point2{polygon->padfX[i + 1], polygon->padfY[i + 1]};
            points_poly.push_back(point1);
            lines_poly.push_back(Line{point1, point2});
        }

        SHPDestroyObject(polygon);
        SHPClose(handle_poly);
    } 
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
    }
    
}

void SurveyScheme::initSurveyLines(double azimuth_grad, double dL)
{
    angle_grad = utils::convertAzimuthToAngle(azimuth_grad);
    angle_rad = utils::convertDegreesToRadians(angle_grad);
    rectangle.initLines(angle_grad, dL);

    int line_id = 1;
    for (const Line& line_rect : rectangle.lines) 
    {
        std::vector<Point> intersections;
        for (const Line& line_poly : lines_poly) 
        {
            Point intersection = Line::getIntersection(line_poly, line_rect);
            if (intersection != utils::POINT_DUMMY)
                intersections.push_back(intersection);
        }
        if ((intersections.size() > 1) 
            && (intersections[0] != intersections[intersections.size() - 1])) 
        {
            std::sort(intersections.begin(), intersections.end());
            int line_sub_id = 0;
            for (size_t i{0}; i < intersections.size() - 1; i+=2) 
            {
                std::string line_name = std::to_string(line_id) + "." + std::to_string(line_sub_id);
                lines_survey[line_name] = Line(intersections[i], intersections[i + 1]);
                line_sub_id++;
            }
            line_id++;
        }
    }
}

void SurveyScheme::initSurveyPoints(double ds)
{
    for (auto [line_name, line] : lines_survey)
    {
        std::vector<Point> stations = planPointsAlongLine(line, ds);
        points_survey[line_name] = stations;
    }
}

std::vector<Point> SurveyScheme::planPointsAlongLine(const Line& line, double ds)
{
    Point point = line.caps.first;
    std::vector<Point> points;
    if (angle_grad == 0) 
    {
        do
        {
            points.push_back(point);
            point.x += ds;
        } while (point < line.caps.second);
    }
    else if (angle_grad == 90)
    {
        do
        {
            points.push_back(point);
            point.y += ds;
        } while (point < line.caps.second);
    }
    else if (angle_grad < 90 && angle_grad > 0)
    {
        do
        {
            points.push_back(point);
            point.x += ds * std::cos(angle_rad);
            point.y += ds * std::sin(angle_rad);
        } while (point < line.caps.second);
    }
    else if (angle_grad > 90 && angle_grad < 180)
    {
        double beta_rad = utils::convertDegreesToRadians(180 - angle_grad);
        do
        {
            points.push_back(point);
            point.x -= ds * std::cos(beta_rad);
            point.y += ds * std::sin(beta_rad);
        } while (point.y < line.caps.second.y);
    }
    return points;
}