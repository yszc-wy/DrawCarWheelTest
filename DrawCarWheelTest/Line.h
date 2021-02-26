#pragma once
#include <vector>

#include "Vertex.h"
#include "ShaderProgram.h"
#include "OpenglPointMsg.h"
namespace OpenglObj {
class Line {
public:
	Line(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices);
	Line(const OpenglPointMsg& point_msg);
	void Draw(ShaderProgram &shaderProgram) const;
private:
	std::vector<Vertex> vertices;//��������
	std::vector<unsigned int> indices;//��������

	unsigned int VAO, VBO,EBO;
	void SetupLine();
};
}