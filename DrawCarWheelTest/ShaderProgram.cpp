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

	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ��֤ifstream��������׳��쳣��
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// ��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// �ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		// ת����������string
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
	//����һ��������ɫ������
	int vertexShader;
	vertexShader = glfun.glCreateShader(GL_VERTEX_SHADER);
	glfun.glShaderSource(vertexShader, 1, &vShaderCode, NULL);//��shaderԴ����
	glfun.glCompileShader(vertexShader);//����shader
								  ////���������
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

	//����һ��Ƭ����ɫ������
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
	//��������ɫ���������ӵ�һ��������Ⱦ����ɫ������(Shader Program)�С�
	ID = glfun.glCreateProgram();
	glfun.glAttachShader(ID, vertexShader);//�Ӵ�
	glfun.glAttachShader(ID, fragmentShader);
	glfun.glLinkProgram(ID);//һ������
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

void ShaderProgram::setBool(const std::string &name, bool value)//�޸������Ա���ߵ����˷ǳ�����:����������Ա���������ݵ������޶���
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
	//�ڶ�����������OpenGL���ǽ�Ҫ���Ͷ��ٸ�����������1
	//����������ѯ�����������Ƿ�ϣ�������ǵľ�������û�(Transpose)��Ҳ����˵�������Ǿ�����к��С�
	//OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�����������(Column - major Ordering)���֡�
	//GLM��Ĭ�ϲ��־������������Բ�����Ҫ�û�����
	//���һ�������������ľ������ݣ�����GLM�����ǰ����ǵľ��󴢴�ΪOpenGL��ϣ�����ܵ����֣�
	//�������Ҫ����GLM���Դ��ĺ���value_ptr���任��Щ���ݡ�
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