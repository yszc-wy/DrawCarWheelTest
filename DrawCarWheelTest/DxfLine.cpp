#include "DxfLine.h"

#include <iostream>
namespace DxfReader {
	void Line::Show() const
	{

	}
	void Line::ShowType() const
	{
		std::cout << "Line:" << std::endl;
	}
	std::string Line::GetType() const
	{
		return "Line";
	}
	std::shared_ptr<IPolygon> Line::clone() const
	{
		return std::make_shared<Line>(*this);
	}
	double Line::GetY(double x) const
	{
		double xmin = this->start_point.x <= this->end_point.x ? this->start_point.x : this->end_point.x;
		double xmax = this->start_point.x + this->end_point.x - xmin;
		if (x<xmin || x>xmax)
		{
			return DBL_MAX;
		}
		double dx = this->end_point.x - this->start_point.x;
		double dy = this->end_point.y - this->start_point.y;
		return ((x - this->start_point.x) / dx)*dy + this->start_point.y;
	}
	void Line::DownStartPoint(double dy)
	{
		this->start_point.y -= dy;
	}
	void Line::DownEndPoint(double dy)
	{
		this->end_point.y -= dy;
	}
	Point Line::GetStartPoint() const
	{
		return this->start_point;
	}
	Point Line::GetEndPoint() const
	{
		return this->end_point;
	}

	std::vector<glm::vec2> Line::GetUniformPoints(Direction direction, unsigned int section) const
	{
		Point actual_start_point;
		Point actual_end_point;

		if (direction == Direction::Forward)
		{
			actual_start_point = this->start_point;
			actual_end_point = this->end_point;
		}
		else if (direction == Direction::Reverse)
		{
			actual_start_point = this->end_point;
			actual_end_point = this->start_point;
		}
		else
		{
			std::cout << "Error::Line::GetUniformPoints::illegal direction" << std::endl;
			return std::vector<glm::vec2>();
		}

		if (section == 0)
		{
			//std::cout << "Error::Line::GetUniformPoints::illegal section" << std::endl;
			//return std::vector<Point>();
			section = 10;
		}
		
		double dx = actual_end_point.x - actual_start_point.x;
		double dy = actual_end_point.y - actual_start_point.y;

		double unit_dx = dx / section;
		double unit_dy = dy / section;
		
		
		std::vector<glm::vec2> points;
		for (int i = 0; i != section + 1; ++i)
		{
			points.push_back(Point(actual_start_point.x + unit_dx*i, actual_start_point.y + unit_dy*i));
		}
		return points;
	}
}