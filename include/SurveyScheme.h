#pragma once

#include "Rectangle.h"
#include "Utils.h"
#include <map>
#include <vector>
#include <string>

class SurveyScheme
{
public:
	Rectangle rectangle;
	std::vector<Line> lines_poly;
	std::vector<Point> points_poly;
	std::map<std::string, Line> lines_survey;
	std::map<std::string, std::vector<Point>> points_survey;
	double angle_grad;
	double angle_rad;
public:
	SurveyScheme(const char* file_path);
	void initSurveyLines(double azimuth_grad, double dL);
	void initSurveyPoints(double ds);
	std::vector<Point> planPointsAlongLine(const Line& line, double ds);
	void saveLinesToShp(const char* file_path);
	//void savePointToShp(const char* file_path);
};