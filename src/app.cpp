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


#include "survey_designer/survey_scheme.hpp"
#include "survey_designer/utils.hpp"
#include <iostream>

int main(int argc, char* argv[]) 
{   
    try {
        utils::SurveyParams survey_params = utils::parse_input_params(argc, argv);
        SurveyScheme survey_scheme{survey_params.path_polygon.c_str()};

        switch (survey_params.type) {
        case utils::SurveyType::LINEAR:
            survey_scheme.init_survey_lines(survey_params.azimuth_grad, survey_params.line_spacing);
            survey_scheme.init_survey_linear_points(survey_params.station_spacing);
            break;
        case utils::SurveyType::HEXAGONAL:
            survey_scheme.init_survey_hex_points(survey_params.station_spacing);
            break;
        default:
            throw std::runtime_error("Wrong survey type. Try again");
            break;
        }

        switch (survey_params.object_to_save) {
        case utils::ObjectToSave::LINE: 
            survey_scheme.save_lines_to_shp(survey_params.path_output.c_str());
            break;
        case utils::ObjectToSave::POINT:
            survey_scheme.save_points_to_shp(survey_params.path_output.c_str(), survey_params.type);
            break;
        case utils::ObjectToSave::ALL:
            survey_scheme.save_lines_to_shp(survey_params.path_output.c_str());
            survey_scheme.save_points_to_shp(survey_params.path_output.c_str(), survey_params.type);
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
