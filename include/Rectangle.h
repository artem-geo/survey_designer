#ifndef RECTANGLE_H
#define RECTANGLE_H


#include "Corners.h"
#include "Line.h"
#include <vector>

class Rectangle
{   
public:
    Corners corners;
    std::vector<Line> sides;
    std::vector<Line> lines;
    
void initRectangle(double xArray[], double yArray[], int nVertices);
void printRectangle();
void initLines(double angleGrad, double dL);
};


#endif