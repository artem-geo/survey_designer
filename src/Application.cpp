#include "Point.h"
#include "Rectangle.h"
#include "ShpInfo.h"
#include "SurveyScheme.h"
#include "Utils.h"
#include <shapefil.h>
#include <iomanip>
#include <iostream>

int main()
{
    double azimuth_grad = 315;
    double dL = 200;
    double ds = 50.0;
    double angle_grad = utils::convertAzimuthToAngle(azimuth_grad);
    const char* file_path = R"(C:\Dev\survey_designer\examples\polygon2.shp)";

    SurveyScheme survey_scheme{file_path};
    survey_scheme.initSurveyLines(azimuth_grad, dL);
    survey_scheme.initSurveyPoints(ds);
    
    std::ofstream ofile("./survey_points_315.csv");
    ofile << "X,Y\n";
   for (auto [line_name, points] : survey_scheme.points_survey) 
   {
        for (const Point& point : points)
        {
            ofile << std::setprecision(3) << std::fixed;
            ofile << point.x << "," << point.y << "\n";
        }
   }

    std::cout << survey_scheme.lines_survey.size() << std::endl;
    std::cout << survey_scheme.rectangle.lines.size() << std::endl;

    //for (auto [line_name, line] : survey_scheme.lines_survey)
    //{
    //    ofile << std::setprecision(3) << std::fixed;
    //    ofile << line.caps.first.x << "," << line.caps.first.y << "\n";
    //    ofile << line.caps.second.x << "," << line.caps.second.y << "\n";
    //}

    //for (auto line : survey_scheme.rectangle.lines)
    //{
    //    ofile << std::setprecision(3) << std::fixed;
    //    ofile << line.caps.first.x << "," << line.caps.first.y << "\n";
    //    ofile << line.caps.second.x << "," << line.caps.second.y << "\n";
    //}

    ofile.close();

    // std::cin.get();
}