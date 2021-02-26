//#ifndef MESH_H
//#define MESH_H
#pragma once

#include <vector>

#include "ShaderProgram.h"
#include "Texture.h"
#include "Vertex.h"
#include "OpenglPointMsg.h"
namespace OpenglObj {
	class Mesh /*: protected QOpenGLFunctions_3_3_Core*/ {
	public:
		/*  ����  */
		Mesh();
		Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices,std::vector<Texture> textures = std::vector<Texture>());
		Mesh(const OpenglPointMsg& point_msg, std::vector<Texture> textures = std::vector<Texture>());
		~Mesh();
		void Draw(ShaderProgram &shaderProgram) const;
		void ShallowCopy(const Mesh &mesh)
		{
			this->VAO = mesh.VAO;
			this->VBO = mesh.VBO;
			this->EBO = mesh.EBO;
		}
		void DeleteMesh();//�ͷ���Դ
		void Show();
		bool get_is_set_up() const;
	private:
		void set_is_set_up(bool t);

		std::vector<Vertex> vertices;//��������
		std::vector<unsigned int> indices;//��������
		std::vector<Texture> textures;//��������

		unsigned int VAO, VBO, EBO;
		void SetupMesh();

		bool is_set_up;
	};
}