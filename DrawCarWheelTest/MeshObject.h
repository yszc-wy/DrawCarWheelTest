#pragma once
#include <vector>
#include <iostream>

#include "Object.h"
#include "Mesh.h"
namespace Object {
	class MeshObject : public Object//世界坐标中的物体
	{
	public:
		MeshObject(glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/) :
			model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/ {}
		MeshObject(const std::vector<OpenglObj::Mesh>& meshes, glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/)
			:meshes(meshes), model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/ {}
		MeshObject(const OpenglObj::Mesh& mesh, glm::mat4 model = glm::mat4(1.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f)/*, float width = STANDARDWIDTH, float radius = STANDARDTREADRADIUS*/) :
			model(model), color(color)/*, cylinderWidth(width), cylinderRadius(radius)*/
		{
			this->meshes.push_back(mesh);
		}
		void DeleteMeshs()
		{
			for (auto &i : this->meshes)
			{
				i.DeleteMesh();
			}
		}
		void Show()
		{
			std::cout << "MeshObject:" << std::endl;
			for (auto &mesh : this->meshes)
			{
				mesh.Show();
			}
		}
		//	Object& operator =(const Object& str)//赋值运算符
		//	{
		//		meshes = str.meshes;
		//		model = str.model;

		//		//cylinderWidth = str.cylinderWidth;
		//		//cylinderRadius = str.cylinderRadius;
		//		return *this;
		//	}
		void AddMesh(const OpenglObj::Mesh& mesh)
		{
			this->meshes.push_back(mesh);
		}
		size_t MeshSize()
		{
			return meshes.size();
		}
		void SetModel(const glm::mat4& model)
		{
			this->model = model;
		}
		void SetColor(const glm::vec3& color)
		{
			this->color = color;
		}
		glm::mat4 GetModel() const
		{
			return this->model;
		}
		void Draw(ShaderProgram& shader) const override
		{
			if (meshes.size() == 0)
			{
				std::cout << "ERROR::MeshObject::NO MESHES CAN BE DRAW!" << std::endl;
			}
			else
			{
				shader.setMat4("model", model);
				shader.setVec3("objectColor", color);
				for (auto &i : meshes)
				{
					i.Draw(shader);
				}
			}
		}

	private:
		std::vector<OpenglObj::Mesh> meshes;
		glm::mat4 model;//世界坐标位置
		glm::vec3 color;//
						//intersect cylinder area
						//float cylinderWidth;
						//float cylinderRadius;
	};
}