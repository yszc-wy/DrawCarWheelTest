#include "Mesh.h"

#include <iostream>

//#include <qopenglfunctions_3_3_core.h>
#include <qopenglfunctions_3_3_compatibility.h>
namespace OpenglObj {
	Mesh::Mesh():vertices(), indices(), textures(), is_set_up(false)
	{
	}

	Mesh::Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices, std::vector<Texture> textures)
		:vertices(vertices), indices(indices), textures(textures), is_set_up(false)
		//:VBO(QOpenGLBuffer::VertexBuffer),EBO(QOpenGLBuffer::IndexBuffer)//�趨buffer����
	{//��Ҫ�����������У���

		//QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();//ÿ��ʹ�ú���ǰ��Ҫ��ʼ��
		//this->textures = textures;
		//std::cout << "MESH:"<< this->vertices.size()<<" "<< this->indices.size() << std::endl;
		//��ʼ��opengl����
		this->SetupMesh();
	}
	Mesh::Mesh(const OpenglPointMsg& point_msg, std::vector<Texture> textures)
		:vertices(point_msg.GetVertices()), indices(point_msg.GetIndices()), textures(textures), is_set_up(false)
	{
		this->SetupMesh();
	}

	void Mesh::set_is_set_up(bool t)
	{
		//std::cout << "Mesh::Set Up::" << t << std::endl;
		this->is_set_up = t;
	}
	bool Mesh::get_is_set_up() const
	{
		return is_set_up;
	}
	void Mesh::Show()
	{
		std::cout << "Mesh:" << std::endl;
		for (auto &vertice : this->vertices)
		{
			std::cout <<"vertice position x:"<<vertice.Position.x <<" y:"<< vertice.Position.y<<" z:"<< vertice.Position.z << std::endl;
		}
		std::cout << std::endl;
	}
	void Mesh::Draw(ShaderProgram &shaderProgram) const//mesh��draw������ϵ��ת����������
	{
		if (!this->is_set_up)
		{
			std::cout << "Mesh::Draw::illegal mesh" << std::endl;
			return;
		}

		//QOpenGLFunctions_3_3_Core glfun;   //��Ҫ�̳У���ֵ��������
		QOpenGLFunctions_3_3_Compatibility glfun;
		glfun.initializeOpenGLFunctions();

		//QOpenGLFunctions_3_3_Core *glfun=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

		//QOpenGLFunctions_3_3_Core *glfun=QOpenGLContext::functions();
		//unsigned int diffuseNr = 1;
		//unsigned int specularNr = 1;
		//unsigned int normalNr = 1;
		//unsigned int heightNr = 1;
		//for (unsigned int i = 0; i < textures.size(); i++)//�󶨲���,Ĭ�ϲ����Ѿ��������
		//{
		//	glfun.glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		//									  // retrieve texture number (the N in diffuse_textureN)
		//	std::string number;
		//	std::string name = textures[i].type;
		//	if (name == "texture_diffuse")
		//		number = std::to_string(diffuseNr++);
		//	else if (name == "texture_specular")
		//		number = std::to_string(specularNr++); // transfer unsigned int to stream
		//	else if (name == "texture_normal")
		//		number = std::to_string(normalNr++); // transfer unsigned int to stream
		//	else if (name == "texture_height")
		//		number = std::to_string(heightNr++); // transfer unsigned int to stream
		//	 
		//											 // now set the sampler to the correct texture unit
		//	shaderProgram.setInt(name + number, i);

		//	glBindTexture(GL_TEXTURE_2D, textures[i].id);
		//}
		if (textures.size() > 0)
		{
			glfun.glActiveTexture(GL_TEXTURE0);
			glfun.glBindTexture(GL_TEXTURE_2D, textures[0].id);
		}

		//std::cout << "Draw ok1" << std::endl;
		// draw mesh
		glfun.glBindVertexArray(VAO);
		glfun.glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glfun.glBindVertexArray(0);


		//std::cout << "Draw ok2" << std::endl;

	}
	void Mesh::SetupMesh()
	{
		//std::cout << "Mesh::SetupMesh ok?" << std::endl;

		//QOpenGLFunctions_3_3_Core glfun;   //��Ҫ�̳У���ֵ��������
		QOpenGLFunctions_3_3_Compatibility glfun;
		glfun.initializeOpenGLFunctions();

		//QOpenGLFunctions_3_3_Core *glfun = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

		//std::cout << "Mesh::SetupMesh 1" << std::endl;
		//this->VAO.create();
		//this->VBO.create();
		//this->EBO.create();
		//
		//this->VAO.bind();
		//
		//this->VBO.bind();
		//this->VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
		//this->VBO.allocate(&vertices[0], vertices.size() * sizeof(Vertex));//������glBufferData
		//
		//this->EBO.bind();
		//this->EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
		//this->EBO.allocate(&indices[0], indices.size() * sizeof(unsigned int));

		glfun.glGenVertexArrays(1, &VAO);
		glfun.glGenBuffers(1, &VBO);
		glfun.glGenBuffers(1, &EBO);

		//std::cout << "Mesh::SetupMesh 2" << std::endl;
		//std::cout << "VAO:" << VAO << " VBO:" << VBO << " EBO:" << EBO << std::endl;

		glfun.glBindVertexArray(VAO);
		glfun.glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glfun.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glfun.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glfun.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);


		//std::cout << "Mesh::SetupMesh 3" << std::endl;

		// ����λ��
		glfun.glEnableVertexAttribArray(0);
		glfun.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// ���㷨��
		glfun.glEnableVertexAttribArray(1);
		glfun.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// ������������
		glfun.glEnableVertexAttribArray(2);
		glfun.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glfun.glBindVertexArray(0);

		this->set_is_set_up(true);
		//std::cout << "Mesh::SetupMesh ok!" << std::endl;

	}
	void Mesh::DeleteMesh()
	{
		if (!this->is_set_up)
		{
			//std::cout << "Warning::Mesh::DeleteMesh::is deleted" << std::endl;
			return;
		}

		//QOpenGLFunctions_3_3_Core glfun;   //��Ҫ�̳У���ֵ��������
		QOpenGLFunctions_3_3_Compatibility glfun;
		glfun.initializeOpenGLFunctions();

		glfun.glDeleteVertexArrays(1, &VAO);
		glfun.glDeleteBuffers(1, &VBO);
		glfun.glDeleteBuffers(1, &EBO);

		//std::cout << "Mesh is delete" << std::endl;
		this->set_is_set_up(false);
	}
	Mesh::~Mesh()
	{

	}
}