#pragma once

#include <glm/glm.hpp>
#include <qpainter.h>

#include "Text.h"
class TextPointObject 
{
public:
	TextPointObject():position(glm::vec3(0.0f,0.0f,0.0f)),model(glm::mat4(1.0f)),text(){}
	TextPointObject(const glm::vec3& position,const glm::mat4&model, Text& text):position(position),model(model),text(text){}
	void Draw(QPainter& painter);
private:
	void LoadScreenPosition(const glm::mat4& projection,const glm::mat4& view);

	glm::vec3 position;
	glm::mat4 model;

	Text text;
	glm::vec2 screen_position;
};