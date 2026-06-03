#pragma once

#include "survey_designer/point.hpp"
#include <string>

namespace utils {
    const Point POINT_DUMMY = Point(-999, -999);
    enum class SurveyType { LINEAR=0, HEXAGONAL=1 };
    enum class ObjectToSave {LINE=0, POINT=1, ALL=2};
    struct SurveyParams {
        SurveyType type;
        ObjectToSave object_to_save;
        double line_spacing;
        double azimuth_grad;
        double station_spacing;
        std::string path_polygon;
        std::string path_output;
    };
    double azimuth2angle(double azimuth);
    double degrees2radians(double angle);
    SurveyParams parse_input_params(int argc, char* argv[]);
    SurveyType parse_type_param(std::string& survey_type_param);
    ObjectToSave parse_obj_to_save_params(std::string& object_to_save_param);
    double parse_line_spacing_param(std::string& line_spacing_param);
    double parse_azimuth_grad_param(std::string& line_azimuth_param);
    double parse_station_spacing_params(std::string& spation_spacing_param);
}
