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
		std::vector<std::pair<std::string, Point>> points_hex_survey;
		double angle_grad;
		double angle_rad;
	private:
		std::vector<Point> planPointsAlongLine(const Line& line, double station_spacing);
		bool checkPointInPolygon(const Point& point);
	public:
		SurveyScheme(const char* file_path);
		void initSurveyLines(double azimuth_grad, double line_spacing);
		void initSurveyLinearPoints(double ds);
		void initSurveyHexPoints(double ds);
		void saveLinesToShp(const char* file_path);
		void savePointsToShp(const char* file_path, utils::SurveyType survey_type);
};