#include "OpenglPointMsg.h"
#include <iostream>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
namespace OpenglObj {
	const std::vector<Vertex>& OpenglPointMsg::GetVertices() const
	{
		return vertices;
	}
	const std::vector<unsigned int>& OpenglPointMsg::GetIndices() const
	{
		return indices;
	}
	void OpenglPointMsg::SetMsg_Move(std::vector<Vertex>&& overtices, std::vector<unsigned int>&& oindices)
	{
		vertices = overtices;
		indices = oindices;
	}
	OpenglPointMsg& OpenglPointMsg::operator+=(const OpenglPointMsg& rhs)
	{
		float start_point_id = this->vertices.size();
		this->vertices.insert(vertices.end(), rhs.vertices.begin(), rhs.vertices.end());
		
		std::vector<unsigned int> add_indices;
		add_indices = rhs.indices;
		for (auto &indice : add_indices)
		{
			indice += start_point_id;
		}
		this->indices.insert(indices.end(), add_indices.begin(), add_indices.end());
		return *this;
	}
	void OpenglPointMsg::AddVertice(const Vertex& vertex)
	{
		this->vertices.push_back(vertex);
	}
	void OpenglPointMsg::AddIndice(unsigned int indice)
	{
		this->indices.push_back(indice);
	}
	//void OpenglPointMsg::AddjustIndices(unsigned int start_id,const std::vector<unsigned int>& indices)
	//{
	//	for (auto &indice : this->indices)
	//	{
	//		indice += start_id;
	//	}
	//}
	OpenglPointMsg operator+(const OpenglPointMsg &lhs, const OpenglPointMsg &rhs)
	{
		OpenglPointMsg sum = lhs;
		sum += rhs;
		return sum;
	}
	OpenglPointMsg LoadCircle(const glm::vec3& circle_center, float radius, int section, const glm::vec3& normal, const glm::vec3& color)
	{
		if (section < 4)
		{
			std::cout << "Error::LoadCircle::illigel section" << std::endl;
		}

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		float unit_radian = 2 * kPIR / float(section);
		glm::vec3 origin_normal = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 target_normal = glm::normalize(normal);

		glm::mat4 rotate(1.0f);
		if (origin_normal != target_normal && origin_normal!=-target_normal)
		{
			glm::vec3 rotate_axis = glm::normalize(glm::cross(origin_normal, target_normal));
			float rotate_radian = glm::angle(origin_normal, normal);
			rotate = glm::rotate(glm::mat4(1.0f), rotate_radian, rotate_axis);//¶ÔÆëfront
		}
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), circle_center);
		glm::mat4 model = translate*rotate;

		for (int i = 0; i != section; ++i)
		{
			float radian = unit_radian*i;
			glm::vec3 origin_position = glm::vec3(radius*cos(radian), radius*sin(radian), 0.0f);

			Vertex circle_point;
			circle_point.Position=glm::vec3(model*glm::vec4(origin_position,1.0f));
			circle_point.Color = color;
			//std::cout << "circle_point:" << circle_point.Position.x << " " << circle_point.Position.y << " " << circle_point.Position.z << std::endl;
			vertices.push_back(circle_point);

			indices.push_back(i);
			indices.push_back((i + 1) % section);
		}
		
		return OpenglObj::OpenglPointMsg(std::move(vertices), std::move(indices));
	}
}