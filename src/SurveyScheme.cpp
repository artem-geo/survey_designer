#include "Point.h"
#include "shapefil.h"
#include "ShpInfo.h"
#include "SurveyScheme.h"
#include <algorithm>
#include <cstring>
#include <iostream>

SurveyScheme::SurveyScheme(const char* file_path)
    : rectangle(Rectangle()), angle_grad(-1), angle_rad(-1), dL(-1), ds(-1) 
{
    auto handle_in = SHPOpen(file_path, "rb");
    ShpInfo shp_info;
    SHPGetInfo(handle_in, &(shp_info.n_entities), &(shp_info.shp_type), shp_info.padf_min_bound, shp_info.padf_max_bound);

    // check the opened SHP: type == 5 (Polygon) and only one polygon
    if (shp_info.shp_type != 5)
        throw std::ios::failure("Please, provide a Polygon SHP file");

    if (shp_info.n_entities != 1)
        throw std::ios::failure("Please, provide a SHP file with ONE polygon");

    auto polygon = SHPReadObject(handle_in, 0);
    rectangle.initRectangle(polygon->padfX, polygon->padfY, polygon->nVertices);
    
    // nVertices - 1 as the first and the last vertices are equal
    for (size_t i{0}; i < ((polygon->nVertices) - 1); ++i) { 
        Point point1{polygon->padfX[i], polygon->padfY[i]};
        Point point2{polygon->padfX[i + 1], polygon->padfY[i + 1]};
        // get polygon nodes and edges
        points_poly.push_back(point1);
        lines_poly.push_back(Line{point1, point2});
    }
    SHPDestroyObject(polygon);
    SHPClose(handle_in);
}

/**
 * @brief Initialises survey lines
 * @param azimuth_grad survey lines angle relative to the east CCW
 * @param dL line spacing
 */
void SurveyScheme::initSurveyLines(double azimuth_grad, double line_spacing) 
{
    dL = line_spacing;
    angle_grad = Utils::convertAzimuthToAngle(azimuth_grad);
    angle_rad = Utils::convertDegreesToRadians(angle_grad);
    rectangle.initLines(angle_grad, dL);

    int line_id = 1;
    for (const Line& line_rect : rectangle.lines) {
        std::vector<Point> intersections;
        for (const Line& line_poly : lines_poly) {
            Point intersection = Line::getIntersection(line_poly, line_rect);
            if (intersection != Utils::POINT_DUMMY)
                intersections.push_back(intersection);
        }
        if ((intersections.size() > 1) && (intersections[0] != intersections[intersections.size() - 1])) {
            std::sort(intersections.begin(), intersections.end());
            int line_sub_id = 0;
            for (size_t i{0}; i < intersections.size() - 1; i+=2) {
                std::string line_name = std::to_string(line_id) + "." + std::to_string(line_sub_id);
                lines_survey[line_name] = Line(intersections[i], intersections[i + 1]);
                line_sub_id++;
            }
            line_id++;
        }
    }
}

/**
 * @brief Initialises survey points
 * @param ds Survey points separation
 */
void SurveyScheme::initSurveyLinearPoints(double station_spacing) 
{
    ds = station_spacing;
    for (auto [line_name, line] : lines_survey) {
        std::vector<Point> stations = planPointsAlongLine(line, ds);
        points_survey[line_name] = stations;
    }
}

/**
 * @brief Initialises hexagonal survey points
 * @param station_spacing double value of survey station spacing
 */
void SurveyScheme::initSurveyHexPoints(double station_spacing) 
{
    angle_grad = 60;
    angle_rad = Utils::convertDegreesToRadians(angle_grad);
    Point point_init = rectangle.corners.bottom_left; // first point of the dataset
    Point point_even = point_init; // first point in an even row
    double dx = station_spacing * std::cos(angle_rad);
    double dy = station_spacing * std::sin(angle_rad);
    Point point_odd; // first point in an odd row
    point_odd.y = point_even.y + dy;
    point_odd.x = point_even.x + dx;
    
    // even and odd rows
    std::vector<Point> points_even;
    std::vector<Point> points_odd;

    // init points in even and odd rows
    do {
        points_even.push_back(point_even);
        point_even.x += station_spacing;

        points_odd.push_back(point_odd);
        point_odd.x += station_spacing;
    } while (point_odd.x < rectangle.corners.bottom_right.x);

    int row_number{0}; // numbering starts at 0
    do {
        if (row_number % 2 == 0) {
            rectangle.points_hex.insert(rectangle.points_hex.end(), points_even.begin(), points_even.end());
            std::for_each(points_even.begin(), points_even.end(),
                [&dy] (Point& p) {p.y += (2 * dy); });
        }
        if (row_number % 2 != 0) {
            rectangle.points_hex.insert(rectangle.points_hex.end(), points_odd.begin(), points_odd.end());
            std::for_each(points_odd.begin(), points_odd.end(),
                [&dy] (Point& p) {p.y += (2 * dy); });
        }
        ++row_number;
    } while ((rectangle.points_hex.end() - 1)->y < rectangle.corners.top_left.y);

    int point_id{1};
    for (const Point& point : rectangle.points_hex) {
        if (checkPointInPolygon(point)) 
            points_hex_survey.push_back({std::to_string(point_id++), point});
    }
}

/**
 * @brief Construct survey points along a survey line
 * @param line line that should be used for the construction
 * @param ds survey point separation
 * @return vector of points along the line
 */
std::vector<Point> SurveyScheme::planPointsAlongLine(const Line& line, double ds) 
{
    Point point = line.caps.first; // first point coincides with the beginning of the line
    std::vector<Point> points;
    if (angle_grad == 0) {
        do {
            points.push_back(point);
            point.x += ds;
        } while (point < line.caps.second);
    }
    else if (angle_grad == 90) {
        do {
            points.push_back(point);
            point.y += ds;
        } while (point < line.caps.second);
    }
    else if (angle_grad < 90 && angle_grad > 0) {
        do {
            points.push_back(point);
            point.x += ds * std::cos(angle_rad);
            point.y += ds * std::sin(angle_rad);
        } while (point < line.caps.second);
    }
    else if (angle_grad > 90 && angle_grad < 180) {
        double beta_rad = Utils::convertDegreesToRadians(180 - angle_grad);
        do {
            points.push_back(point);
            point.x -= ds * std::cos(beta_rad);
            point.y += ds * std::sin(beta_rad);
        } while (point.y < line.caps.second.y);
    }
    return points;
}

/**
 * @brief Checks if a point is within the polygon
 * @param point point to check
 * @return true - point within, false - outside
 */
bool SurveyScheme::checkPointInPolygon(const Point& point) 
{
    Point ray_cap_right {rectangle.corners.bottom_right.x + 100, point.y};
    Line ray{point, ray_cap_right}; // init ray starting at the point and running to the right 
    int number_intersections{0};
    for (const Line& line : lines_poly) { // loop counting number of intersections
        if (Line::checkSegmentsIntersection(ray, line))
            number_intersections++;
    }
    // even intersections = point outside; odd = inside
    return (number_intersections % 2 == 0) ? false : true;
}

/**
 * @brief Saves constructed survey lines into SHP, SHX and DBF files
 * @param file path C-string
 */
void SurveyScheme::saveLinesToShp(const char* file_path) 
{
    std::string file_path_string = file_path;
    file_path_string.append(R"(\lines)");
    if (lines_survey.size() == 0) {
        std::cerr << "No lines constructed. Construct the lines at first" << std::endl;
        throw std::exception();
    }
    auto handle_out_shp = SHPCreate(file_path_string.c_str(), SHPT_ARC);
    auto handle_out_dbf = DBFCreate(file_path_string.c_str());
    int ifield_lid = DBFAddField(handle_out_dbf, "LINEID", FTString, 10, 0);
    int ifield_x1 = DBFAddField(handle_out_dbf, "X1", FTDouble, 10, 3);
    int ifield_y1 = DBFAddField(handle_out_dbf, "Y1", FTDouble, 10, 3);
    int ifield_x2 = DBFAddField(handle_out_dbf, "X2", FTDouble, 10, 3);
    int ifield_y2 = DBFAddField(handle_out_dbf, "Y2", FTDouble, 10, 3);
    int ifield_len = DBFAddField(handle_out_dbf, "LEN", FTDouble, 10, 3);
    
    for (auto [line_name, line] : lines_survey) {

        const double padf_x[] = {line.caps.first.x, line.caps.second.x};
        const double padf_y[] = {line.caps.first.y, line.caps.second.y}; 
        const double padf_z[] = {0.0, 0.0}; 
        auto line_obj = SHPCreateSimpleObject(SHPT_ARC, 2, padf_x, padf_y, padf_z);
        int ishape = SHPWriteObject(handle_out_shp, -1, line_obj);

        DBFWriteStringAttribute(handle_out_dbf, ishape, ifield_lid, line_name.c_str());
        DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_x1, line.caps.first.x);
        DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_y1, line.caps.first.y);
        DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_x2, line.caps.second.x);
        DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_y2, line.caps.second.x);
        DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_len, distance(line.caps.first, line.caps.second));
        
        SHPDestroyObject(line_obj);
    }
    SHPClose(handle_out_shp);
    DBFClose(handle_out_dbf);
}

/**
 * @brief Saves stations to the SHP file
 * @param file_path C-string file path
 */
void SurveyScheme::savePointsToShp(const char* file_path, Utils::SurveyType survey_type) 
{
    std::string file_path_string = file_path;
    if (survey_type == Utils::SurveyType::LINEAR) {
        file_path_string.append(R"(\points)");
        if (points_survey.size() == 0) {
            std::cerr << "No points constructed. Construct the points at first" << std::endl;
            throw std::exception();
        }
    }
    if (survey_type == Utils::SurveyType::HEXAGONAL) {
        file_path_string.append(R"(\points_hex)");
        if (points_hex_survey.size() == 0) {
            std::cerr << "No points constructed. Construct the points at first" << std::endl;
            throw std::exception();
        }
    }

    auto handle_out_shp = SHPCreate(file_path_string.c_str(), SHPT_POINT);
    auto handle_out_dbf = DBFCreate(file_path_string.c_str());
    int ifield_sid = DBFAddField(handle_out_dbf, "STID", FTString, 10, 0);
    int ifield_x = DBFAddField(handle_out_dbf, "X", FTDouble, 10, 3);
    int ifield_y = DBFAddField(handle_out_dbf, "Y", FTDouble, 10, 3);

    if (survey_type == Utils::SurveyType::LINEAR) {
        int ifield_lid = DBFAddField(handle_out_dbf, "LINEID", FTString, 10, 0);

        for (auto [line_name, points] : points_survey) {
            int point_id{0};

            for (const Point& point : points) {
                const double padf_x[] = {point.x,};
                const double padf_y[] = {point.y,};
                const double padf_z[] = {0.0,};

                auto point_obj = SHPCreateSimpleObject(SHPT_POINT, 1, padf_x, padf_y, padf_z);
                int ishape = SHPWriteObject(handle_out_shp, -1, point_obj);

                DBFWriteStringAttribute(handle_out_dbf, ishape, ifield_lid, line_name.c_str());
                DBFWriteStringAttribute(handle_out_dbf, ishape, ifield_sid, std::to_string(point_id).c_str());
                DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_x, point.x);
                DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_y, point.y);
                SHPDestroyObject(point_obj);
                point_id += ds;
            }
        }
    }
    if (survey_type == Utils::SurveyType::HEXAGONAL) {
        for (auto [point_id, point] : points_hex_survey) {
            const double padf_x[] = {point.x,};
            const double padf_y[] = {point.y,};
            const double padf_z[] = {0.0,};

            auto point_obj = SHPCreateSimpleObject(SHPT_POINT, 1, padf_x, padf_y, padf_z);
            int ishape = SHPWriteObject(handle_out_shp, -1, point_obj);

            DBFWriteStringAttribute(handle_out_dbf, ishape, ifield_sid, point_id.c_str());
            DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_x, point.x);
            DBFWriteDoubleAttribute(handle_out_dbf, ishape, ifield_y, point.y);
            SHPDestroyObject(point_obj);
        }
    }
    SHPClose(handle_out_shp);
    DBFClose(handle_out_dbf); 
}