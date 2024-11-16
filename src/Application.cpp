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
    const char* file_path_in = R"(C:\Dev\survey_designer\examples\input\polygon)";
    const char* file_path_out_lines = R"(C:\Dev\survey_designer\examples\output\lines)";
    const char* file_path_out_points = R"(C:\Dev\survey_designer\examples\output\points)";

    SurveyScheme survey_scheme{file_path_in};
    survey_scheme.initSurveyLines(azimuth_grad, dL);
    survey_scheme.initSurveyPoints(ds);
    survey_scheme.saveLinesToShp(file_path_out_lines);
    survey_scheme.savePointsToShp(file_path_out_points);
    
}