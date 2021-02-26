#include "Text.h"

#include <iostream>
void Text::set_text(const std::string& text)
{
	this->text = text;
}
void Text::set_size(float size)
{
	this->size = size;
}
void Text::set_color(const glm::vec3& color)
{
	//if (color.x < 0 || color.x>1
	//	|| color.y < 0 || color.y>1
	//	|| color.z < 0 || color.z>1)
	//{
	//	std::cout << "ColorGradientStrip::Draw::illegal color" << std::endl;
	//	return;
	//}
	this->color = color;
}

std::string Text::get_text() const
{
	return this->text;
}
float Text::get_size() const
{
	return this->size;
}
glm::vec3 Text::get_color() const
{
	return this->color;
}