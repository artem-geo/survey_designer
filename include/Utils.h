#pragma once

#include "Point.h"

namespace utils
{
    const Point POINT_DUMMY = Point(-999, -999);
    enum class SurveyType { LINEAR=0, HEXAGONAL=1 };
    enum class ObjectToSave {LINE=0, POINT=1, ALL=2};
    struct SurveyParams
    {
        SurveyType type;
        ObjectToSave object_to_save;
        double line_spacing;
        double azimuth_grad;
        double station_spacing;
        const char* path_polygon;
        const char* path_output;
    };
    double convertAzimuthToAngle(double azimuth);
    double convertDegreesToRadians(double angle);
    SurveyParams parseInputParams(int argc, char* argv[]);
    SurveyType parseSurveyTypeParam(std::string& survey_type_param);
    ObjectToSave parseObjectToSaveParam(std::string& object_to_save_param);
    double parseLineSpacingParam(std::string& line_spacing_param);
    double parseAzimuthGradParam(std::string& line_azimuth_param);
}