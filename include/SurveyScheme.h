#pragma once

#include "Rectangle.h"
#include "Utils.h"
#include <map>
#include <vector>
#include <string>

class SurveyScheme
{
private:
	Rectangle rectangle;
	double dL;
	double ds;
	std::vector<Line> lines_poly;
	std::vector<Point> points_poly;
	std::map<std::string, Line> lines_survey;
	std::map<std::string, std::vector<Point>> points_survey;
	double angle_grad;
	double angle_rad;
private:
	std::vector<Point> planPointsAlongLine(const Line& line, double station_spacing);
public:
	SurveyScheme(const char* file_path);
	void initSurveyLines(double azimuth_grad, double line_spacing);
	void initSurveyPoints(double ds);
	void saveLinesToShp(const char* file_path);
	void savePointsToShp(const char* file_path);
};