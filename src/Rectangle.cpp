#include "Point.h"
#include "Rectangle.h"
#include "Utils.h"
#include <cmath>
#include <iostream>
#include <numbers>
   
void Rectangle::initRectangle(double x_array[], double y_array[], int n_vertices)
{
    Point bottom_left{};
    Point top_left{};
    Point top_right{};
    Point bottom_right{};
    double x_min{-1}, y_min{-1}, x_max{-1}, y_max{-1};

    for (size_t i{0}; i < (n_vertices-1); ++i) // nVertices = nVertices(actual) + 1
    { 
        if (i == 0) 
        {
            x_min = x_max = x_array[i];
            y_min = y_max = y_array[i];
            continue;
        }
        x_min = (x_min > x_array[i]) ? x_array[i] : x_min;
        x_max = (x_max < x_array[i]) ? x_array[i] : x_max;
        y_min = (y_min > y_array[i]) ? y_array[i] : y_min;
        y_max = (y_max < y_array[i]) ? y_array[i] : y_max;
    }

    corners.bottom_left  = {x_min, y_min};
    corners.top_left = {x_min, y_max};
    corners.top_right = {x_max, y_max};
    corners.bottom_right = {x_max, y_min};

    sides.push_back(Line(corners.bottom_left, corners.top_left));
    sides.push_back(Line(corners.top_left, corners.top_right));
    sides.push_back(Line(corners.top_right, corners.bottom_right));
    sides.push_back(Line(corners.bottom_right, corners.bottom_left));
}

void Rectangle::printRectangle()
{
    for (size_t i{0}; i < sides.size(); i++) 
    {
        std::cout << "Line " << i << ": " << sides.at(i) << "\n";
    }
}

void Rectangle::initLines(double angle_grad, double dL)
{
    Point point_begin, point_end;
    double angle_rad = utils::convertDegreesToRadians(angle_grad);
    if (angle_grad == 0) 
    {
        point_begin = corners.bottom_left;
        point_end = corners.bottom_right;
        do 
        {
            lines.push_back(Line(point_begin, point_end));
            point_begin.y += dL;
            point_end.y += dL;
        } while (point_begin.y <= corners.top_left.y);
    }
    else if (angle_grad == 90) 
    {
        point_begin = corners.bottom_left;
        point_end = corners.top_left;
        do 
        {
            lines.push_back(Line(point_begin, point_end));
            point_begin.x += dL;
            point_end.x += dL;
        } while (point_begin.x <= corners.bottom_right.x);
    }
    else if (angle_grad < 90 && angle_grad > 0) 
    {
        // central point of a line
        Point point_central = corners.bottom_right;
        
        // delta x and y to calculate line caps from the central point
        double x_delta_caps = corners.bottom_right.x - corners.bottom_left.x;
        double y_delta_caps = x_delta_caps * std::tan(angle_rad);

        // angle between the rectangle's diagonal and the bottom edge
        double beta_rad = std::atan((corners.top_right.y - corners.bottom_right.y) / (x_delta_caps));
        // step of central point along the diagonal
        double delta_diagonal = dL / std::sin(angle_rad + beta_rad);

        // delta x and y to calculate new coord-s of the central point
        double x_delta_central = delta_diagonal * std::sin(beta_rad);
        double y_delta_central = delta_diagonal * std::cos(beta_rad);

        // push line and update central point until central point is within the rectangle
        while ((point_central.x > corners.bottom_left.x) 
            || (point_central.y < corners.top_left.y)) 
        {
            lines.push_back(Line(Point(point_central.x - x_delta_caps, point_central.y - y_delta_caps), 
                                Point(point_central.x + x_delta_caps, point_central.y + y_delta_caps)));
            point_central.x -= x_delta_central;
            point_central.y += y_delta_central;
        }            
    }
    else if (angle_grad > 90 && angle_grad < 180) 
    {
        angle_rad = std::numbers::pi - angle_rad;

        // central point of a line
        Point point_central = corners.bottom_left;
        
        // delta x and y to calculate line caps from the central point
        double x_delta_caps = corners.bottom_right.x - corners.bottom_left.x;
        double y_delta_caps = x_delta_caps * std::tan(angle_rad);

        // angle between the rectangle's diagonal and the bottom edge
        double beta_rad = std::atan((corners.top_right.y - corners.bottom_right.y) / (x_delta_caps));
        // step of central point along the diagonal
        double delta_diagonal = dL / std::sin(angle_rad + beta_rad);

        // delta x and y to calculate new coord-s of the central point
        double x_delta_central = delta_diagonal * std::sin(beta_rad);
        double y_delta_central = delta_diagonal * std::cos(beta_rad);

        // push line and update central point until central point is within the rectangle
        while ((point_central.x < corners.bottom_right.x) 
            || (point_central.y < corners.top_right.y)) 
        {
            lines.push_back(Line(Point(point_central.x + x_delta_caps, point_central.y - y_delta_caps), 
                                Point(point_central.x - x_delta_caps, point_central.y + y_delta_caps)));
            point_central.x += x_delta_central;
            point_central.y += y_delta_central;
        }   
    }
}