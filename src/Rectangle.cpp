#include "Point.h"
#include "Rectangle.h"
#include "Utils.h"
#include <cmath>
#include <iostream>
   
void Rectangle::initRectangle(double xArray[], double yArray[], int nVertices)
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

void Rectangle::printRectangle()
{
    for (size_t i{0}; i < sides.size(); i++)
    {
        std::cout << "Line " << i << ": " << sides.at(i) << "\n";
    }
}

void Rectangle::initLines(double angleGrad, double dL)
{
    Point pointBegin, pointEnd;
    double angleRad = utils::convertDegreesToRadians(angleGrad);
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