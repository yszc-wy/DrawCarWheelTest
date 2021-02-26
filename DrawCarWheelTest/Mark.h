#pragma once

#include <glm/glm.hpp>

#include "Text.h"
namespace Coordinate {
	class Mark
	{
	public:
		explicit Mark(float position_ratio, const Text& text) :position_ratio(position_ratio), line_color(glm::vec3(1.0f,1.0f,1.0f)), text(text)
		{
		}
		explicit Mark(float position_ratio,glm::vec3 mark_color, const Text& text) :position_ratio(position_ratio), line_color(mark_color), text(text)
		{
		}

		float position_ratio;
		glm::vec3 line_color;
		Text text;
	};
}