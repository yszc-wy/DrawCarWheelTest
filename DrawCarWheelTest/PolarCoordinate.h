#pragma once
#include <vector>

#include "Mark.h"
#include "Texture.h"
#include "OpenglPointMsg.h"
#include "CoordinateObject.h"

namespace Coordinate {
	const float kPIR = 3.1415926f;
	class PolarCoordinate {
	public:
		static const float kAngleMarkExtend;
		static const float kAngleMarkTextDisToMarkEnd;
		static const float kCircleMarkTextDisToMark;
		//static const float kZAxisMarkTextDisToAxisEnd;

		explicit PolarCoordinate():radius(50.0f),color(glm::vec3(1.0f,1.0f,1.0f)), /*z_axis_length(100.0f),*/angle_marks(), circle_marks() {}
		explicit PolarCoordinate(float radius):radius(radius), color(glm::vec3(1.0f, 1.0f, 1.0f)), /*z_axis_length(100.0f),*/ angle_marks(),circle_marks(){}

		void set_radius(float radius);
		void set_color(const glm::vec3& color);
		void AddAngleMark(const Mark& mark);
		void AddCircleMark(const Mark& mark);
		Object::CoordinateObject LoadObject(const Texture& font_texture);
	private:

		float radius;
		glm::vec3 color;
		//float z_axis_length;
		std::vector<Mark> angle_marks;
		std::vector<Mark> circle_marks;
	};
}