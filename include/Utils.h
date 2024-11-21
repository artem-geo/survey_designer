#pragma once

#include "point.h"

namespace Utils {
    const Point POINT_DUMMY = Point(-999, -999);
    enum class Survey_type { LINEAR=0, HEXAGONAL=1 };
    enum class Object_to_save {LINE=0, POINT=1, ALL=2};
    struct Survey_params {
        Survey_type type;
        Object_to_save object_to_save;
        double line_spacing;
        double azimuth_grad;
        double station_spacing;
        std::string path_polygon;
        std::string path_output;
    };
    double convert_azimuth_to_angle(double azimuth);
    double convert_degrees_to_radians(double angle);
    Survey_params parse_input_params(int argc, char* argv[]);
    Survey_type parse_type_param(std::string& survey_type_param);
    Object_to_save parse_object_to_save_param(std::string& object_to_save_param);
    double parse_line_spacing_param(std::string& line_spacing_param);
    double parse_azimuth_grad_param(std::string& line_azimuth_param);
    double parse_station_spacing_param(std::string& spation_spacing_param);
}