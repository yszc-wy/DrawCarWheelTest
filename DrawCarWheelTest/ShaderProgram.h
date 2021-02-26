#pragma once
#include <string>
#include <glm/glm.hpp>
//#include <qopenglfunctions_3_3_core.h>
#include <qopenglfunctions_3_3_compatibility.h>
class ShaderProgram
{
public:
	// 程序ID
	unsigned int ID;
	ShaderProgram(){}
	// 构造器读取并构建着色器
	ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 使用/激活程序
	void use();
	// uniform工具函数
	void setBool(const std::string &name, bool value) ;
	void setInt(const std::string &name, int value) ;
	void setFloat(const std::string &name, float value) ;
	void setMat4(const std::string &name, glm::mat4 vec) ;
	void setVec3(const std::string &name, float x, float y, float z) ;
	void setVec3(const std::string &name, glm::vec3 vec) ;
	void setVec4(const std::string &name, float x, float y, float z,float w) ;
	void setVec4(const std::string &name, glm::vec4 vec) ;
	void setVec2(const std::string &name, glm::vec2 vec);

	void initShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);


};