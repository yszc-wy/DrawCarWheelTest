#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <qmessagebox.h>


ShaderProgram::ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	this->initShaderProgram(vertexPath, fragmentPath);
}

void ShaderProgram::initShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();

	// 1. 从文件路径中获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		QString error = "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
		//std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	//创建一个顶点着色器对象
	int vertexShader;
	vertexShader = glfun.glCreateShader(GL_VERTEX_SHADER);
	glfun.glShaderSource(vertexShader, 1, &vShaderCode, NULL);//绑定shader源代码
	glfun.glCompileShader(vertexShader);//编译shader
								  ////检测编译错误
	int  success;
	char infoLog[512];
	glfun.glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glfun.glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		QString error = "ERROR::SHADER::VERTEX::COMPILATION_FAILED";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//创建一个片段着色器对象
	int fragmentShader;
	fragmentShader = glfun.glCreateShader(GL_FRAGMENT_SHADER);
	glfun.glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glfun.glCompileShader(fragmentShader);
	glfun.glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glfun.glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		QString error = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//把两个着色器对象链接到一个用来渲染的着色器程序(Shader Program)中。
	ID = glfun.glCreateProgram();
	glfun.glAttachShader(ID, vertexShader);//接触
	glfun.glAttachShader(ID, fragmentShader);
	glfun.glLinkProgram(ID);//一起链接
					  // check for linking errors
	glfun.glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glfun.glGetProgramInfoLog(ID, 512, NULL, infoLog);
		QString error = "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
		//std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glfun.glDeleteShader(vertexShader);
	glfun.glDeleteShader(fragmentShader);
}

void ShaderProgram::use()
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	glfun.glUseProgram(ID);
}

void ShaderProgram::setBool(const std::string &name, bool value)//修改了类成员或者调用了非常函数:对象包含与成员函数不兼容的类型限定符
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	glfun.glUniform1i(glfun.glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderProgram::setInt(const std::string &name, int value)
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	glfun.glUniform1i(glfun.glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string &name, float value) 
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	glfun.glUniform1f(glfun.glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderProgram::setMat4(const std::string &name, glm::mat4 vec) 
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	glfun.glUniformMatrix4fv(glfun.glGetUniformLocation(ID, name.c_str()) , 1, GL_FALSE, glm::value_ptr(vec));
	//第二个参数告诉OpenGL我们将要发送多少个矩阵，这里是1
	//第三个参数询问我们我们是否希望对我们的矩阵进行置换(Transpose)，也就是说交换我们矩阵的行和列。
	//OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column - major Ordering)布局。
	//GLM的默认布局就是列主序，所以并不需要置换矩阵。
	//最后一个参数是真正的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望接受的那种，
	//因此我们要先用GLM的自带的函数value_ptr来变换这些数据。
}
void ShaderProgram::setVec3(const std::string &name, float x, float y, float z) 
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	glfun.glUniform3f(glfun.glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void ShaderProgram::setVec2(const std::string &name, glm::vec2 vec)
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	float* value = glm::value_ptr(vec);
	glfun.glUniform2f(glfun.glGetUniformLocation(ID, name.c_str()), value[0], value[1]);
}

void ShaderProgram::setVec3(const std::string &name, glm::vec3 vec) 
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	float* value = glm::value_ptr(vec);
	glfun.glUniform3f(glfun.glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2]);
}
void ShaderProgram::setVec4(const std::string &name, float x, float y, float z, float w) 
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	glfun.glUniform4f(glfun.glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
void ShaderProgram::setVec4(const std::string &name, glm::vec4 vec) 
{
	//QOpenGLFunctions_3_3_Core glfun;
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();
	float* value = glm::value_ptr(vec);
	glfun.glUniform4f(glfun.glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2], value[3]);
}