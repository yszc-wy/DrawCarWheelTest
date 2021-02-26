//#ifndef OBJECT_H
//#define OBJECT_H
#pragma once
//#include <vector>
//#include <iostream>
//
//#include "Mesh.h"
//#include "Line.h"
//#include "Wheel.h"
#include "ShaderProgram.h"
namespace Object {
	class Object
	{
	public:
		virtual void Draw(ShaderProgram& shader) const = 0;
		virtual ~Object() = default;
	};
}
//class MeshObject : public Object//世界坐标中的物体
//{
//public:
//	MeshObject(glm::mat4 model = glm::mat4(1.0f),glm::vec3 color=glm::vec3(1.0f,1.0f,1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/):
//        model(model),color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/ {}
//	MeshObject(const std::vector<OpenglObj::Mesh>& meshes, glm::mat4 model=glm::mat4(1.0f),glm::vec3 color=glm::vec3(1.0f,1.0f,1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/)
//        :meshes(meshes), model(model),color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/ {}
//	MeshObject(const OpenglObj::Mesh& mesh, glm::mat4 model = glm::mat4(1.0f),glm::vec3 color=glm::vec3(1.0f,1.0f,1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/):
//        model(model),color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/
//	{
//		this->meshes.push_back(mesh);
//	}
////	Object& operator =(const Object& str)//赋值运算符
////	{
////		meshes = str.meshes;
////		model = str.model;
//
////		//cylinderWidth = str.cylinderWidth;
////		//cylinderRadius = str.cylinderRadius;
////		return *this;
////	}
//	void AddMesh(const OpenglObj::Mesh& mesh)
//	{
//		this->meshes.push_back(mesh);
//	}
//	size_t MeshSize()
//	{
//		return meshes.size();
//	}
//	void SetModel(glm::mat4 model)
//	{
//		this->model = model;
//	}
//    void SetColor(glm::vec3 color)
//    {
//        this->color=color;
//    }
//	glm::mat4 GetModel() const
//	{
//		return this->model;
//	}
//	void Draw(ShaderProgram& shader) const override
//	{
//		if (meshes.size() == 0)
//		{
//			std::cout << "ERROR::MeshObject::NO MESHES CAN BE DRAW!" << std::endl;
//		}
//		else
//		{
//			shader.setMat4("model", model);
//            shader.setVec3("objectColor", color);
//			for (auto &i : meshes)
//			{
//				i.Draw(shader);
//			}
//		}
//	}
//
//private:
//	std::vector<OpenglObj::Mesh> meshes;
//	glm::mat4 model;//世界坐标位置
//    glm::vec3 color;//
//	//intersect cylinder area
//	//float cylinderWidth;
//	//float cylinderRadius;
//};
//class LineObject :public Object
//{
//public:
//	LineObject(glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/) 
//		:lines(),model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/ {}
//	LineObject(const std::vector<OpenglObj::Line>& lines, glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/)
//		:lines(lines), model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/ {}
//	LineObject(const OpenglObj::Line& line, glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/) :
//		model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/
//	{
//		this->lines.push_back(line);
//	}
//	void AddLine(const OpenglObj::Line& line)
//	{
//		this->lines.push_back(line);
//	}
//	size_t LineSize()
//	{
//		return lines.size();
//	}
//	void SetModel(glm::mat4 model)
//	{
//		this->model = model;
//	}
//	//void SetColor(glm::vec3 color)
//	//{
//	//	this*->color =l color;
//	//}
//	glm::mat4 GetModel() const
//	{
//		return this->model;
//	}
//	void Draw(ShaderProgram& shader) const override
//	{
//		if (lines.size() == 0)
//		{
//			std::cout << "ERROR::LineObject::NO Lines CAN BE DRAW!" << std::endl;
//		}
//		else
//		{
//			shader.setMat4("model", model);
//			shader.setVec3("objectColor", color);
//			for (auto &i : lines)
//			{
//				i.Draw(shader);
//			}
//		}
//	}
//
//private:
//	std::vector<OpenglObj::Line> lines;  //一般情况下只有一个
//	glm::mat4 model;//世界坐标位置
//	glm::vec3 color;
//};
////聚合类，表示一些特定事物
//class CoordinateObject
//{
//public:
//	CoordinateObject(const LineObject& coordinate_line,const std::vector<MeshObject>&mark_texts):coordinate_line(coordinate_line),mark_texts(mark_texts){}
//	void UpdateTextDirection()
//	{
//
//	}
//	void SetModel(const glm::mat4& model)//调整整体
//	{
//
//	}
//	void Draw(ShaderProgram& line_shader,ShaderProgram& font_shader)
//	{
//		coordinate_line.Draw(line_shader);
//		for (auto &i : mark_texts)
//		{
//			i.Draw(font_shader);
//		}
//	}
//private:
//	LineObject coordinate_line;
//	std::vector<MeshObject> mark_texts;//每个text需要不同的model
//};
