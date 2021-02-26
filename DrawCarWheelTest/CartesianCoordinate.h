#pragma once
#include <string>

#include "Mark.h"
#include "CoordinateObject.h"
#include "Range.h"
namespace Coordinate {
	//象限
	const unsigned char kCoordinateSingleOctant =0x01;//单卦限
	const unsigned char kCoordinateHalfOctant =0x0f;//半卦限
	const unsigned char kCoordinateWholeOctant =0xff;//全卦限
	//坐标
	enum Axis{X,Y,Z};
	enum MarkType{ X_PLUS_MARK,X_MINUS_MARK,Y_PLUS_MARK,Y_MINUS_MARK, ZOX_PLUS_MARK,ZOX_MINUS_MARK,ZOY_PLUS_MARK,ZOY_MINUS_MARK};

	class CartesianCoordinate
	{
	public:
		static const float kAxisTextDisToAxisEnd;
		static const float kXAxisMarkTextDisToMarkEnd;
		static const float kYAxisMarkTextDisToMarkEnd;
		static const float kZAxisMarkTextDisToAxis;
		static const float kTextMarkExtend;
		static const float kAxisExtend;
		static const float kNoTextMarkExtend;
		static const float kArrowWidth;
		static const float kArrowLength;

		CartesianCoordinate()
			:
			axis_extend(kAxisExtend),
			axis_text_dis_to_axis_end(kAxisTextDisToAxisEnd),
			arrow_length(kArrowLength),
			arrow_width(kArrowWidth),

			axis_length(glm::vec3(100.0f, 100.0f,100.0f)),
			x_axis_color(glm::vec3(1.0f,1.0f,1.0f)),
			y_axis_color(glm::vec3(1.0f,1.0f,1.0f)),
			z_axis_color(glm::vec3(1.0f,1.0f,1.0f)),
			x_axis_text(Text("X", 8, x_axis_color)),
			y_axis_text(Text("Y", 8, y_axis_color)),
			z_axis_text(Text("z", 8, z_axis_color)),
			coordinate_octant(kCoordinateSingleOctant)
		{}
		//axis length
		void set_axis_length(Axis axis,float length);
		void set_axis_length(float x_length, float y_length, float z_length);
		//axis text
		void set_axis_text(Axis axis,const Text& text);
		void set_axis_text(const Text& x_text, const Text& y_text, const Text& z_text);
		//axis color
		void set_axis_color(Axis axis, const glm::vec3& color);
		void set_axis_color(const glm::vec3& x_color, const glm::vec3& y_color, const glm::vec3& z_color);
		//coordinate quadrant
		void set_coordinate_octant(unsigned char coordinate_octant);
		//add mark
		void AddMark(Axis axis, const Mark& mark);
		//load
		Object::CoordinateObject LoadObject(const Texture& font_texture);

		//距离调整
		void set_axis_extend(float axis_extend);
		void set_axis_text_dis_to_axis_end(float axis_text_dis_to_axis_end);
		void set_arrow_width(float arrow_width);
		
	private:
		bool Judge_Octant(int q_id);
		void GetAxisRange(Range *range);
		void GetMarkRange(Range *range);
		//调整参数
		float axis_extend;
		float axis_text_dis_to_axis_end;
		float arrow_length;
		float arrow_width;

		//基本属性
		glm::vec3 axis_length;
		glm::vec3 x_axis_color;
		glm::vec3 y_axis_color;
		glm::vec3 z_axis_color;
		Text x_axis_text;
		Text y_axis_text;
		Text z_axis_text;
		unsigned char coordinate_octant;
		std::vector<Mark> x_axis_marks;
		std::vector<Mark> y_axis_marks;
		std::vector<Mark> z_axis_marks;
	};
}