#pragma once

#include "survey_designer/rectangle.hpp"
#include "survey_designer/utils.hpp"
#include <map>
#include <string>
#include <vector>

class SurveyScheme {
private:
	Rectangle rectangle;
	double dL;
	double ds;
	std::vector<Line> lines_poly;
	std::vector<Point> points_poly;
	std::map<std::string, Line> lines_survey;
	std::map<std::string, std::vector<Point>> points_survey;
	std::vector<std::pair<std::string, Point>> points_hex_survey;
	double angle_grad;
	double angle_rad;
private:
	std::vector<Point> plan_points_along_line(const Line& line, double station_spacing);
	bool check_point_in_polygon(const Point& point);
public:
	SurveyScheme(const char* file_path);
	void init_survey_lines(double azimuth_grad, double line_spacing);
	void init_survey_linear_points(double ds);
	void init_survey_hex_points(double ds);
	void save_lines_to_shp(const char* file_path);
	void save_points_to_shp(const char* file_path, utils::SurveyType survey_type);
};