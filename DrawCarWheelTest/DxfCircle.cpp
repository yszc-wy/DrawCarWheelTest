#include "DxfCircle.h"

#include <iostream>
namespace DxfReader {
	void Circle::ShowType() const
	{
		std::cout << "Circle:" << std::endl;
	}
	std::string Circle::GetType() const
	{
		return "Circle";
	}
	std::shared_ptr<IPolygon> Circle::clone() const
	{
		return std::make_shared<Circle>(*this);
	}
	void Circle::DownStartPoint(double dy)
	{
		std::cout << "Warning::Circle::DownStartPoint" << std::endl;
	}
	void Circle::DownEndPoint(double dy)
	{
		std::cout << "Warning::Circle::DownEndPoint" << std::endl;
	}
	double Circle::GetY(double x) const
	{
		//this->ShowType();
		//this->GetStartPoint().Show();
		//this->GetEndPoint().Show();

		//std::cout << "Circle::GetY::1" << std::endl;
		if (x<this->center_point.x - r || x>this->center_point.x + r)
		{
			return DBL_MAX;
		}
		/*if (x == cx - r)
		{
			double y = cy;
			double angle = 180;
			if (InAngleRange(angle))
			{
				return y;
			}
			else
			{
				return DBL_MAX;
			}
		}

		if (x == cx + r)
		{
			double y = cy;
			double angle = 0;
			if (InAngleRange(angle))
			{
				return y;
			}
			else
			{
				return DBL_MAX;
			}
		}*/
		//std::cout << "Circle::GetY::2" << std::endl;

		double item = sqrt(pow(r, 2) - pow(x - this->center_point.x, 2));
		double y1 = item + this->center_point.y;
		double y2 = -item + this->center_point.y;

		/*std::cout << "Circle::GetY::3" << std::endl;
		std::cout << "y1:" << y1 << " y2:" << y2 << std::endl;
		std::cout << "center_point x:" << this->center_point.x << " y:" << this->center_point.y << std::endl;

		std::cout << " GetAnglePoint x:" << x << " y:" << y1 << std::endl;*/
		double angle1=GetPointAngleToBasePoint(x - this->center_point.x, y1 - this->center_point.y);

		//std::cout << " GetAnglePoint x:" << x << " y:" << y2 << std::endl;
		double angle2=GetPointAngleToBasePoint(x - this->center_point.x, y2 - this->center_point.y);
		
		//std::cout << "angle1 :" << angle1 << " angle2:" << angle2 << std::endl;
		//std::cout << std::endl;

		bool in_range1 = InAngleRange(angle1);
		bool in_range2 = InAngleRange(angle2);

		

		if (in_range1&&in_range2)//如果两个点都在范围就返回一个最大的
		{
			return y1 >= y2 ? y1 : y2;
		}
		else if (in_range1)
		{
			return y1;
		}
		else if (in_range2)
		{
			return y2;
		}
		else
		{
			return DBL_MAX;
		}
	}

	bool Circle::InAngleRange(double angle) const
	{
		double d_angle = this->start_angle;

		double fix_end_angle = ConvertToStdAngle(this->end_angle - d_angle);
		double fix_angle = ConvertToStdAngle(angle - d_angle);

		if (fix_angle >= 0 && fix_angle <= fix_end_angle)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Point Circle::GetStartPoint() const
	{
		Point start_point=GetPointFromPolar(ConvertToRadian(this->start_angle), this->r, this->center_point.x, this->center_point.y);
		return start_point;
	}
	Point Circle::GetEndPoint() const
	{
		Point end_point = GetPointFromPolar(ConvertToRadian(this->end_angle), this->r, this->center_point.x, this->center_point.y);
		return end_point;
	}
	std::vector<glm::vec2> Circle::GetUniformPoints(Direction direction, unsigned int section) const
	{
		double angle_range = ConvertToStdAngle(this->end_angle - this->start_angle);
		double unit_angle;

		if (section == 0)//按角度取值
		{
			unit_angle = 1;
			section = angle_range / unit_angle;
		}
		else
		{
			double unit_angle = angle_range / section;
		}

		std::vector<glm::vec2> points;
		if (direction == Direction::Forward)
		{
			for (int i = 0; i != section + 1; ++i)
			{
				Point point=GetPointFromPolar(ConvertToRadian(this->start_angle + unit_angle*i), this->r, this->center_point.x, this->center_point.y);
				points.push_back(point);
			}
			return points;
		}
		else if (direction == Direction::Reverse)
		{
			for (int i = 0; i != section + 1; ++i)
			{
				Point point = GetPointFromPolar(ConvertToRadian(this->end_angle - unit_angle*i), this->r, this->center_point.x, this->center_point.y);
				points.push_back(point);
			}
			return points;
		}
		else
		{
			std::cout << "Error::Circle::GetUniformPoints::illegal direction" << std::endl;
			return std::vector<glm::vec2>();
		}
	}

	double ConvertToStdAngle(double angle)
	{
		if (angle < 0.0f) {
			while (1) {
				angle += 360.0f;
				if (angle >= 0.0) break;
			}
		}
		else if (angle >= 360.0f) {
			while (1) {
				angle -= 360.0f;
				if (angle < 360.0f) break;
			}
		}
		return angle;
	}

	double ConvertToAngle(double radian)
	{
		return 360 * radian / (2 * kPIR);
	}

	double ConvertToRadian(double angle)
	{
		return 2 * kPIR*angle / 360.0f;
	}

	double GetPointAngleToBasePoint(double x, double y)
	{
		if (x==0&&y==0)
		{
			return DBL_MAX;
		}
		//std::cout << "GetPointAngleToBasePoint:: x:" << x << " y:" << y << std::endl;

		double r = sqrt(pow(x, 2) + pow(y, 2));
		
		double basis_angle = ConvertToAngle(acos(x / r));
		//std::cout << "basis_angle:" << basis_angle << std::endl;

		if (y >= 0)
		{
			double angle = basis_angle;
			return angle;
		}
		else
		{
			double angle = 360 - basis_angle;
			return angle;
		}
	}

	Point GetPointFromPolar(double radian, double r, double m_x, double m_y)
	{
		Point p;
		p.x = r*cos(radian)+ m_x;
		p.y = r*sin(radian)+ m_y;
		return p;
	}

}

