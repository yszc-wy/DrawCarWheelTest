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
	std::vector<Vertex> vertices;//顶点数据
	std::vector<unsigned int> indices;//索引数据

	unsigned int VAO, VBO,EBO;
	void SetupLine();
};
}