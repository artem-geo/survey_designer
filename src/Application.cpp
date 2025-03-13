/*
Survey planner v 1.0
Input parameters

Parameter 0. Survey planner file

Parameter 1. Survey type
    -c = conventional survey
    -h = hexagonal survey



If Parameter 1 == -c:

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
        -fs = output file path

If Parameter 1 == -h:
    
    Parameter 2. Station spacing
        -dl= station spacing

    Parameter 3.
        -fp = polygon file path excluding file extenstion, ie. polygon, not polygon.shp

    Parameter 4.
        -fs = output file path excluding file extenstion, ie. scheme, not scheme.shp


Example:
    surveyplanner.exe -c -b -dl=50 -a=45 -ds=10 C:\Users\User\Desktop\tempfile C:\Users\User\Desktop\output

*/


#include "survey_scheme.h"
#include "utils.h"
#include <iostream>

int main(int argc, char* argv[]) 
{   
    try {
        Utils::SurveyParams survey_params = Utils::parseInputParams(argc, argv);
        SurveyScheme survey_scheme{survey_params.path_polygon.c_str()};

        switch (survey_params.type) {
        case Utils::SurveyType::LINEAR:
            survey_scheme.initSurveyLines(survey_params.azimuth_grad, survey_params.line_spacing);
            survey_scheme.initSurveyLinearPoints(survey_params.station_spacing);
            break;
        case Utils::SurveyType::HEXAGONAL:
            survey_scheme.initSurveyHexPoints(survey_params.station_spacing);
            break;
        default:
            throw std::runtime_error("Wrong survey type. Try again");
            break;
        }

        switch (survey_params.object_to_save) {
        case Utils::ObjectToSave::LINE: 
            survey_scheme.saveLinesToShp(survey_params.path_output.c_str());
            break;
        case Utils::ObjectToSave::POINT:
            survey_scheme.savePointsToShp(survey_params.path_output.c_str(), survey_params.type);
            break;
        case Utils::ObjectToSave::ALL:
            survey_scheme.saveLinesToShp(survey_params.path_output.c_str());
            survey_scheme.savePointsToShp(survey_params.path_output.c_str(), survey_params.type);
            break;
        default:
            throw std::runtime_error("Wrong object to save. Try again");
            break;
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        std::exit(1);
    }    
}