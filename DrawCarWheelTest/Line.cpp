#include "Line.h"
#include <iostream>

//#include <qopenglfunctions_3_3_core.h>
#include <qopenglfunctions_3_3_compatibility.h>
namespace OpenglObj {
	Line::Line(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices)
		:vertices(vertices), indices(indices)
	{
		this->SetupLine();
	}
	Line::Line(const OpenglPointMsg& point_msg)
		:vertices(point_msg.GetVertices()), indices(point_msg.GetIndices())
	{
		this->SetupLine();
	}
	void Line::Draw(ShaderProgram &shaderProgram) const
	{
		//QOpenGLFunctions_3_3_Core glfun;   //不要继承，赋值会有问题
		QOpenGLFunctions_3_3_Compatibility glfun;
		glfun.initializeOpenGLFunctions();

		glfun.glBindVertexArray(VAO);
		glfun.glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
		glfun.glBindVertexArray(0);
	}
	void Line::SetupLine()
	{
		//QOpenGLFunctions_3_3_Core glfun;   //不要继承，赋值会有问题
		QOpenGLFunctions_3_3_Compatibility glfun;
		glfun.initializeOpenGLFunctions();

		glfun.glGenVertexArrays(1, &VAO);
		glfun.glGenBuffers(1, &VBO);
		glfun.glGenBuffers(1, &EBO);

		//std::cout << "VAO:" << VAO << " VBO:" << VBO << " EBO:" << EBO << std::endl;

		glfun.glBindVertexArray(VAO);
		glfun.glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glfun.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glfun.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glfun.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);


		// 顶点位置
		glfun.glEnableVertexAttribArray(0);
		glfun.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// 顶点法线
		glfun.glEnableVertexAttribArray(1);
		glfun.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// 顶点纹理坐标
		glfun.glEnableVertexAttribArray(2);
		glfun.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// 顶点颜色
		glfun.glEnableVertexAttribArray(3);
		glfun.glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
		glfun.glBindVertexArray(0);
	}
}