#pragma once
#include <vector>
#include <string>

#include "Line.h"
#include "Text.h"
#include "Mark.h"
#include "CoordinateObject.h"

namespace Coordinate {

	class Ruler
	{
	public:
		static const float kMarkLength;
		static const float kMarkTextDisToMark;

		explicit Ruler();
		explicit Ruler(float length);
		explicit Ruler(float length, const std::vector<Mark>& marks);

		void set_length(float length);
		void set_color(const glm::vec3 color);
		void add_mark(Mark mark);//±Í≥ﬂŒª÷√
		Object::CoordinateObject LoadObject(const Texture& font_texture);
		void set_mark_text_dis_to_mark(float mark_text_dis_to_mark);
	private:
		float length;
		glm::vec3 color;
		std::vector<Mark> marks;
		float mark_text_dis_to_mark;
	};
}