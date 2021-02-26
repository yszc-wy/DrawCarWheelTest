#pragma once
#include <string>
#include <iostream>

#include <glm/glm.hpp>
class Text
{
public:
	explicit Text():text("NULL"),size(10),color(glm::vec3(1.0f,1.0f,1.0f)){}
	explicit Text(const std::string& text,float size,const glm::vec3& color):text(text),size(size),color(color)
	{
		//if (this->color.x < 0 || this->color.x>1
		//	|| this->color.y < 0 || this->color.y>1
		//	|| this->color.z < 0 || this->color.z>1)
		//{
		//	std::cout << "ColorGradientStrip::Draw::illegal color recover to 1.0f 1.0f 1.0f" << std::endl;
		//	this->color = glm::vec3(1.0f, 1.0f, 1.0f);
		//}
	}
	explicit Text(const std::string& text, float size) :text(text), size(size), color(glm::vec3(1.0f,1.0f,1.0f)) {}

	void set_text(const std::string& text);
	void set_size(float size);
	void set_color(const glm::vec3& color);
	std::string get_text() const;
	float get_size() const;
	glm::vec3 get_color() const;


private:
	std::string text;
	float size;
	glm::vec3 color;
};