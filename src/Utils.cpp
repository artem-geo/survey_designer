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
double Utils::convertAzimuthToAngle(double azimuth) 
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
double Utils::convertDegreesToRadians(double angle) 
{
    return angle * std::numbers::pi / 180.0;
}

/**
 * @brief Parses input parameters = number of arguments and C string array
 * @param argc number of parameters
 * @param argv C string array including various parameters
 * @return SurveyParams object containing information about the survey and necessary parameters
 */
Utils::Survey_params Utils::parseInputParams(int argc, char* argv[]) 
{
    if ((argc != 8) && (argc != 5)) 
        throw std::runtime_error("Wrong number of arguments");

    using namespace Utils;
    Survey_params params;

    std::vector<std::string> argvs{argv, argv + argc};

    params.type = parseTypeParam(argvs[1]);
    
    if (params.type == SurveyType::LINEAR) {
        params.object_to_save = parseObjectToSaveParam(argvs[2]);
        params.line_spacing = parseLineSpacingParam(argvs[3]);
        params.azimuth_grad = parseAzimuthGradParam(argvs[4]);
        params.station_spacing = parseStationSpacingParam(argvs[5]);
        params.path_polygon = argvs[6];
        params.path_output = argvs[7];
    }

    if (params.type == SurveyType::HEXAGONAL) {
        params.station_spacing = parseStationSpacingParam(argvs[2]);
        params.object_to_save = ObjectToSave::POINT;
        params.line_spacing = -1;
        params.azimuth_grad = -1;
        params.path_polygon = argvs[3];
        params.path_output = argvs[4];
    }
    return params;
}

Utils::SurveyType Utils::parseTypeParam(std::string& survey_type_param) 
{
    if ((survey_type_param.length() != 2) || survey_type_param[0] != '-')
        throw std::runtime_error("Wrong survey type parameter. Try again");

    switch (survey_type_param[1]) {
    case 'c':
        return SurveyType::LINEAR;
    case 'h':
        return SurveyType::HEXAGONAL;
    default:
        throw std::runtime_error("Wrong survey type parameter. Try again");
    }
}

Utils::ObjectToSave Utils::parseObjectToSaveParam(std::string& survey_type_param) 
{
    if ((survey_type_param.length() != 2) || survey_type_param[0] != '-')
        throw std::runtime_error("Wrong object to save parameter. Try again");

    switch (survey_type_param[1]) {
    case 'l':
        return ObjectToSave::LINE;
    case 'p':
        return ObjectToSave::POINT;   
    case 'b':
        return ObjectToSave::ALL;
    default:
        throw std::runtime_error("Wrong object to save parameter. Try again");
    }
}

double Utils::parseLineSpacingParam(std::string& line_spacing_param) 
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

double Utils::parseAzimuthGradParam(std::string& line_azimuth_param) 
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

double Utils::parseStationSpacingParam(std::string& station_spacing_param) 
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