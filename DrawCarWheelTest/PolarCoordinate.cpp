#include "PolarCoordinate.h"
#include <iostream>
#include <cmath>

#include "Font.h"
#include "MeshObject.h"

namespace Coordinate {
	const float PolarCoordinate::kAngleMarkExtend = 4.5;
	const float PolarCoordinate::kAngleMarkTextDisToMarkEnd=30.0f;
	const float PolarCoordinate::kCircleMarkTextDisToMark=30.0f;

	void PolarCoordinate::set_radius(float radius)
	{
		if (radius <= 0)
		{
			std::cout << "Error::PolarCoordinate::set_radius::illegal radius" << std::endl;
		}
		this->radius = radius;
	}
	void PolarCoordinate::set_color(const glm::vec3& color)
	{
		this->color = color;
	}
	void PolarCoordinate::AddAngleMark(const Mark& mark)
	{
		if (mark.position_ratio < 0 || mark.position_ratio>=1)//不需要360度，0度就可以表示
		{
			std::cout << "Error::PolarCoordinate::AddAngleMark::illegal position_ratio" << std::endl;
		}
		this->angle_marks.push_back(mark);
	}
	void PolarCoordinate::AddCircleMark(const Mark& mark)
	{
		if (mark.position_ratio <0 || mark.position_ratio>1)//0和1是特殊点
		{
			std::cout << "Error::PolarCoordinate::AddCircleMark::illegal position_ratio" << std::endl;
		}
		this->circle_marks.push_back(mark);
	}
	Object::CoordinateObject PolarCoordinate::LoadObject(const Texture& font_texture)
	{
		if (this->radius <= 0)
		{
			std::cout << "Error::PolarCoordinate::LoadObject::illegal radius" << std::endl;
			return Object::CoordinateObject();
		}
		OpenglObj::OpenglPointMsg whole_pmsg;
		//line
		//main circle line
		OpenglObj::OpenglPointMsg main_circle_pmsg = OpenglObj::LoadCircle(glm::vec3(0, 0, 0), this->radius, 50, glm::vec3(0.0f, 0.0f, 1.0f),this->color);
		whole_pmsg += main_circle_pmsg;
		//mark circle line
		for (auto &mark : this->circle_marks)
		{
			if (mark.position_ratio <0 || mark.position_ratio>1)
			{
				std::cout << "Error::PolarCoordinate::LoadObject::Circle_marks::illegal position_ratio" << std::endl;
				continue;
			}
			if (mark.position_ratio == 0|| mark.position_ratio==1)//0,1不需要画，但text要表示出来
			{
				continue;
			}

			float circle_radius = this->radius*mark.position_ratio;
			auto circle_mark_pmsg= OpenglObj::LoadCircle(glm::vec3(0, 0, 0), circle_radius, 50, glm::vec3(0.0f, 0.0f, 1.0f),mark.line_color);
			whole_pmsg += circle_mark_pmsg;
		}
		//mark line
		OpenglObj::OpenglPointMsg angle_mark_msg;
		//Vertex origin_point;
		//unsigned int origin_pid = 0;
		//origin_point.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		//angle_mark_msg.AddVertice(origin_point);

		for (auto &mark : this->angle_marks)
		{
			if (mark.position_ratio < 0 || mark.position_ratio>=1)
			{
				std::cout << "Error::PolarCoordinate::LoadObject::Angle_marks::illegal position_ratio" << std::endl;
				continue;
			}
			float radian = 2.0f*kPIR*mark.position_ratio;

			Vertex circle_center_point;
			circle_center_point.Position = glm::vec3(0.0f, 0.0f, 0.0f);
			circle_center_point.Color = mark.line_color;
			Vertex mark_point;
			mark_point.Position = glm::vec3((radius + kAngleMarkExtend)*cos(radian), (radius + kAngleMarkExtend)*sin(radian), 0.0f);
			mark_point.Color = mark.line_color;

			auto start_pid = angle_mark_msg.GetVertices().size();
			angle_mark_msg.AddVertice(circle_center_point);
			angle_mark_msg.AddVertice(mark_point);

			angle_mark_msg.AddIndice(start_pid);
			angle_mark_msg.AddIndice(start_pid+1);
		}
		whole_pmsg += angle_mark_msg;

		//z axis
		//Vertex z_point;
		//z_point.Position = glm::vec3(0.0f, 0.0f, this->z_axis_length);

		//unsigned int z_start_point_id= whole_pmsg.GetVertices().size();
		//whole_pmsg.AddVertice(origin_point);
		//whole_pmsg.AddVertice(z_point);
		//whole_pmsg.AddIndice(z_start_point_id);
		//whole_pmsg.AddIndice(z_start_point_id+1);

		Object::LineObject coords_line(whole_pmsg, glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		//test
		std::vector<Object::MeshObject> mark_texts;
		std::vector<glm::vec3> mark_text_origin_positions;

		//angle mark test
		for (auto &mark : this->angle_marks)
		{
			if (mark.position_ratio < 0 || mark.position_ratio >= 1)
			{
				std::cout << "Error::PolarCoordinate::LoadObject::Angle_marks::illegal position_ratio" << std::endl;
				continue;
			}
			if (mark.text.get_text() == "")
			{
				continue;
			}
			float radian = 2.0f*kPIR*mark.position_ratio;

			glm::vec3 text_position = glm::vec3((radius + kAngleMarkExtend + kAngleMarkTextDisToMarkEnd)*cos(radian), (radius + kAngleMarkExtend + kAngleMarkTextDisToMarkEnd)*sin(radian), 0.0f);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(mark.text.get_text(), text_position, mark.text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), mark.text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}
		//circle mark test
		for (auto &mark : this->circle_marks)
		{
			if (mark.position_ratio <0 || mark.position_ratio>1)
			{
				std::cout << "Error::PolarCoordinate::LoadObject::Angle_marks::illegal position_ratio" << std::endl;
				continue;
			}
			if (mark.text.get_text() == "")
			{
				continue;
			}
			float circle_radius = this->radius*mark.position_ratio;
			glm::vec3 text_position = glm::vec3(circle_radius,0.0f,-kCircleMarkTextDisToMark);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(mark.text.get_text(), text_position, mark.text.get_size());
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), mark.text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}

		return Object::CoordinateObject(coords_line, mark_texts, mark_text_origin_positions);
	}
}