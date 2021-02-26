#include "Ruler.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "Vertex.h"
#include "Font.h"
namespace Coordinate {
	const float Ruler::kMarkLength = 4.5f;
	const float Ruler::kMarkTextDisToMark = 4.5f;
	Ruler::Ruler()
		:length(0.0f), marks(), color(glm::vec3(1.0f,1.0f,1.0f))
	{
	}
	Ruler::Ruler(float length)
		: length(length), marks(), mark_text_dis_to_mark(kMarkTextDisToMark),color(glm::vec3(1.0f,1.0f,1.0f))
	{

	}
	Ruler::Ruler(float length, const std::vector<Mark>& marks)
		: length(length), marks(marks), mark_text_dis_to_mark(kMarkTextDisToMark)
	{
	}
	void Ruler::set_color(const glm::vec3 color)
	{
		this->color = color;
	}
	void Ruler::set_length(float length)
	{
		if (length <= 0)
		{
			std::cout << "Error::Ruler::set_length::illegal length" << std::endl;
			return;
		}
		this->length = length;
	}
	void Ruler::set_mark_text_dis_to_mark(float mark_text_dis_to_mark)
	{
		this->mark_text_dis_to_mark = mark_text_dis_to_mark;
	}

	void Ruler::add_mark(Mark mark)//标尺位置
	{
		if (mark.position_ratio < 0 || mark.position_ratio > 1)
		{
			std::cout << "Error::Ruler::set_length::illegal position_ratio" << std::endl;
			return;
		}
		this->marks.push_back(mark);
	}
	Object::CoordinateObject Ruler::LoadObject(const Texture& font_texture)
	{
		if (this->length <= 0)
		{
			std::cout << "Error::Ruler::LoadRulerPoint::illegal length" << std::endl;
			return Object::CoordinateObject();
		}

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Vertex startPoint;//0
		startPoint.Position = glm::vec3(0.0f, 0.0f, 0.0f);
		startPoint.Color = this->color;

		vertices.push_back(startPoint);


		Vertex endPoint;//1
		endPoint.Position = glm::vec3(0.0f + length, 0.0f, 0.0f);
		endPoint.Color = this->color;

		vertices.push_back(endPoint);

		//Vertex startMark;//2
		//startMark.Position = glm::vec3(0.0f, kMarkLength, 0.0f);
		//vertices.push_back(startMark);

		//Vertex endMark;//3
		//endMark.Position = glm::vec3(0.0f + length, kMarkLength, 0.0f);
		//vertices.push_back(endMark);

		indices.push_back(0);//主线
		indices.push_back(1);

		//indices.push_back(0);//开始mark
		//indices.push_back(2);
		//
		//indices.push_back(1);//结束mark
		//indices.push_back(3);

		for (auto &mark : this->marks)
		{
			Vertex rootPoint;//1
			rootPoint.Position = glm::vec3(0.0f + length*mark.position_ratio, 0.0f, 0.0f);
			rootPoint.Color = mark.line_color;

			auto rootPointId = vertices.size();
			vertices.push_back(rootPoint);

			Vertex markPoint;//2
			markPoint.Position = glm::vec3(0.0f + length*mark.position_ratio, kMarkLength, 0.0f);
			markPoint.Color = mark.line_color;

			auto markPointId = vertices.size();
			vertices.push_back(markPoint);

			indices.push_back(rootPointId);//mark
			indices.push_back(markPointId);
		}

		//std::cout << "Get Line!" << std::endl;
		Object::LineObject coords_line(OpenglObj::Line(vertices, indices), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));//


		std::vector<Object::MeshObject> mark_texts;
		std::vector<glm::vec3> mark_text_origin_positions;

		//std::cout << "Get Mesh!" << std::endl;


		for (auto &mark : this->marks)
		{
			std::string text = mark.text.get_text();
			glm::vec3 text_position = glm::vec3(0.0f + length*mark.position_ratio, kMarkLength + this->mark_text_dis_to_mark, 0.0f);
			OpenglObj::OpenglPointMsg font_point_msg = Font::LoadFontPoint(text, text_position, mark.text.get_size());

			//float zoom = 1.0f*mark.text.get_size();
			//glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, zoom));//放缩
			Object::MeshObject text_mesh(OpenglObj::Mesh(font_point_msg, std::vector<Texture>{font_texture}), glm::mat4(1.0f), mark.text.get_color());
			mark_texts.push_back(text_mesh);
			mark_text_origin_positions.push_back(text_position);
		}

		//Object::CoordinateObject a(coords_line, mark_texts);
		//a.vertices = vertices;
		//a.indices = indices;

		return Object::CoordinateObject(coords_line, mark_texts, mark_text_origin_positions);
	}
}