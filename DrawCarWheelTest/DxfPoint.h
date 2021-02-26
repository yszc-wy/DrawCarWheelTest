#pragma once
#include <iostream>
#include <string>

#include <glm/glm.hpp>
namespace DxfReader {
	class Point {
		//friend std::ostream& operator<<(std::ostream &out, Point& p);
	public:
		Point(double x=0.0f,double y=0.0f):x(x),y(y){}
		operator glm::vec2() const {
			return glm::vec2(this->x,this->y);
		}
		void Show()
		{
			std::cout << "(" << x << "," << y << ")";
		}
		std::string Get_Log() const
		{
			return "("+std::to_string(x) + "," + std::to_string(y) + ")";
		}
		//	out << "Point(" << p.x << "," << p.y << ")";
		//	return out;
		double x;
		double y;
	};

	//std::ostream & operator<<(std::ostream &out, Point& p)
	//{
	//	out << "Point(" << p.x << "," << p.y << ")";
	//	return out;
	//}
}

