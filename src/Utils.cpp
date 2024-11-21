#include "Utils.h"
#include <cmath>
#include <string>
#include <iostream>
#include <numbers>
#include <vector>

/**
 * @brief Converts azimuth deg to angle deg
 * @param azimuth azimuth angle [0, 360] deg
 * @return [0, 180) deg where 0 deg - East, 90 deg - North
 */
double Utils::convert_azimuth_to_angle(double azimuth)
{
    if (azimuth >= 0 && azimuth <= 90)
        azimuth = 90 - azimuth;
    else if (azimuth > 90 && azimuth <= 270)
        azimuth = 90 - azimuth + 180;
    else if (azimuth > 270 && azimuth <= 360)
        azimuth = 270 - azimuth + 180;
    return azimuth;
}

/**
 * @brief converts angle in degree to angle in radians
 * @param angle angle (degree)
 * @return angle (radians)
 */
double Utils::convert_degrees_to_radians(double angle)
{
    return angle * std::numbers::pi / 180.0;
}

/**
 * @brief Parses input parameters = number of arguments and C string array
 * @param argc number of parameters
 * @param argv C string array including various parameters
 * @return SurveyParams object containing information about the survey and necessary parameters
 */
Utils::Survey_params Utils::parse_input_params(int argc, char* argv[])
{
    if ((argc != 8) && (argc != 5)) 
        throw std::runtime_error("Wrong number of arguments");

    using namespace Utils;
    Survey_params params;

    std::vector<std::string> argvs{argv, argv + argc};

    params.type = parse_type_param(argvs[1]);
    
    if (params.type == Survey_type::LINEAR) {
        params.object_to_save = parse_object_to_save_param(argvs[2]);
        params.line_spacing = parse_line_spacing_param(argvs[3]);
        params.azimuth_grad = parse_azimuth_grad_param(argvs[4]);
        params.station_spacing = parse_station_spacing_param(argvs[5]);
        params.path_polygon = argvs[6];
        params.path_output = argvs[7];
    }

    if (params.type == Survey_type::HEXAGONAL) {
        params.station_spacing = parse_station_spacing_param(argvs[2]);
        params.object_to_save = Object_to_save::POINT;
        params.line_spacing = -1;
        params.azimuth_grad = -1;
        params.path_polygon = argvs[3];
        params.path_output = argvs[4];
    }
    return params;
}

Utils::Survey_type Utils::parse_type_param(std::string& survey_type_param)
{
    if ((survey_type_param.length() != 2) || survey_type_param[0] != '-')
        throw std::runtime_error("Wrong survey type parameter. Try again");

    switch (survey_type_param[1]) {
    case 'c':
        return Survey_type::LINEAR;
    case 'h':
        return Survey_type::HEXAGONAL;
    default:
        throw std::runtime_error("Wrong survey type parameter. Try again");
    }
}

Utils::Object_to_save Utils::parse_object_to_save_param(std::string& survey_type_param)
{
    if ((survey_type_param.length() != 2) || survey_type_param[0] != '-')
        throw std::runtime_error("Wrong object to save parameter. Try again");

    switch (survey_type_param[1]) {
    case 'l':
        return Object_to_save::LINE;
    case 'p':
        return Object_to_save::POINT;   
    case 'b':
        return Object_to_save::ALL;
    default:
        throw std::runtime_error("Wrong object to save parameter. Try again");
    }
}

double Utils::parse_line_spacing_param(std::string& line_spacing_param)
{
    if (line_spacing_param.substr(0, 4) != "-dl=")
        throw std::runtime_error("Wrong line spacing parameter. Try again");
    
    size_t digit_length = line_spacing_param.length() - 4;
    double line_spacing{0.0};

    try {
        line_spacing = std::stod(line_spacing_param.substr(4, digit_length));
    }
    catch (...) {
        throw std::invalid_argument("Wrong line spacing argument. Try again");
    }
    
    if (line_spacing <= 0)
        throw std::runtime_error("Invalid line spacing. Try again");

    return line_spacing;
}

double Utils::parse_azimuth_grad_param(std::string& line_azimuth_param)
{
    if ((line_azimuth_param.substr(0, 3) != "-a="))
        throw std::runtime_error("Wrong azimuth parameter. Try again");

    size_t digit_length = line_azimuth_param.length() - 3;
    double azimuth_grad{0.0};
    try {
        azimuth_grad = std::stod(line_azimuth_param.substr(3, digit_length));
    }
    catch (...) {
        throw std::invalid_argument("Wrong azimuth parameter. Try again");
    }

    if (azimuth_grad < 0)
        throw std::runtime_error("Invalid azimuth. Try again");

    return azimuth_grad;
}

double Utils::parse_station_spacing_param(std::string& station_spacing_param)
{
    if (station_spacing_param.substr(0, 4) != "-ds=")
        throw std::runtime_error("Wrong station spacing parameter. Try again");

    size_t digit_length = station_spacing_param.length() - 4;
    double station_spacing{0.0};
    try {
        station_spacing = std::stod(station_spacing_param.substr(4, digit_length));
    }
    catch (...) {
        throw std::invalid_argument("Wrong station spacing argument. Try again");
    }

    if (station_spacing <= 0)
        throw std::runtime_error("Invalid station spacing. Try again");

    return station_spacing;
}