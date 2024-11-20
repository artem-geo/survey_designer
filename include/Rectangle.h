#pragma once

#include "Corners.h"
#include "Line.h"
#include <vector>

class Rectangle
{   
public:
    Corners corners;
    std::vector<Line> sides;
    std::vector<Line> lines;
    std::vector<Point> points_hex;
    
    void initRectangle(double x_array[], double y_array[], int n_vertices);
    void printRectangle();
    void initLines(double angle_grad, double dL);
};