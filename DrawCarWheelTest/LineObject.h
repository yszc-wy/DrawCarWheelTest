#pragma once
#include <vector>
#include <iostream>

#include "Object.h"
#include "Line.h"
namespace Object {
	class LineObject :public Object
	{
	public:
		LineObject(glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/)
			:lines(), model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/ {}
		LineObject(const std::vector<OpenglObj::Line>& lines, glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/)
			:lines(lines), model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/ {}
		LineObject(const OpenglObj::Line& line, glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/) :
			model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/
		{
			this->lines.push_back(line);
		}
		void AddLine(const OpenglObj::Line& line)
		{
			this->lines.push_back(line);
		}
		size_t LineSize()
		{
			return lines.size();
		}
		void SetModel(glm::mat4 model)
		{
			this->model = model;
		}
		//void SetColor(glm::vec3 color)
		//{
		//	this*->color =l color;
		//}
		glm::mat4 GetModel() const
		{
			return this->model;
		}
		void Draw(ShaderProgram& shader) const override
		{
			if (lines.size() == 0)
			{
				std::cout << "ERROR::LineObject::NO Lines CAN BE DRAW!" << std::endl;
			}
			else
			{
				shader.setMat4("model", model);
				shader.setVec3("objectColor", color);
				for (auto &i : lines)
				{
					i.Draw(shader);
				}
			}
		}

	private:
		std::vector<OpenglObj::Line> lines;  //一般情况下只有一个
		glm::mat4 model;//世界坐标位置
		glm::vec3 color;
	};
}