//#ifndef POLAR_H
//#define POLAR_H
#pragma once
#include <glm/glm.hpp>
class Polar
{
public:
	Polar():r(0), radian(0){}
	Polar(float r,float radian):r(r), radian(radian){}
	glm::vec2 ConvertToXY() const
	{
		glm::vec2 vec;
		vec.x = r*cos(radian);
		vec.y = r*sin(radian);
		return vec;
	}
	float r;
	float radian;

};

