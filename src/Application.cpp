/*
Survey planner v 1.0
Input parameters

Parameter 0. Survey planner file

Parameter 1. Survey type
    -c = conventional survey
    -h = hexagonal survey

Parameter 2. Objects to save
    -l = save lines only
    -p = save points only
    -b = save lines and points

Parameter 3. Line spacing
    -dl = line spacing | if parameter 1 = -h any value will be overwritten

Parameter 4. Line azimuth
    -a = azimuth in degrees (CW). Negative values are not allowed. if parameter 1 = -h any value will be overwritten

Parameter 5. Station spacing
    -ds = station spacing

Parameter 6. 
    -fp = polygon file path excluding file extenstion, ie. polygon, not polygon.shp

Parameter 7.
    -fs = output file path excluding file extenstion, ie. scheme, not scheme.shp

Example:
    surveyplanner.exe -c -b -dl=50 -a=45 -ds=10 C:\Users\User\Desktop\tempfile C:\Users\User\Desktop\output

*/


#include "SurveyScheme.h"
#include "Utils.h"
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        utils::SurveyParams survey_params = utils::parseInputParams(argc, argv);
        std::cout << static_cast<int>(survey_params.type) << std::endl;
        std::cout << static_cast<int>(survey_params.object_to_save) << std::endl;
        std::cout << survey_params.line_spacing << std::endl;
        std::cout << survey_params.azimuth_grad << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        std::exit(1);
    }

    //double azimuth_grad = 315;
    //double dL = 200;
    //double ds = 50.0;
    //double angle_grad = utils::convertAzimuthToAngle(azimuth_grad);
    //const char* file_path_in = R"(C:\Dev\survey_designer\examples\input\polygon)";
    //const char* file_path_out_lines = R"(C:\Dev\survey_designer\examples\output\lines)";
    //const char* file_path_out_points = R"(C:\Dev\survey_designer\examples\output\points)";

    //SurveyScheme survey_scheme{file_path_in};
    //survey_scheme.initSurveyLines(azimuth_grad, dL);
    //survey_scheme.initSurveyPoints(ds);
    //survey_scheme.saveLinesToShp(file_path_out_lines);
    //survey_scheme.savePointsToShp(file_path_out_points);
    
}