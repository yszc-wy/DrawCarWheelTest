#pragma once

#include "OpenglPointMsg.h"
#include "Texture.h"
namespace Font {
	const float kFontWidth=1.0f;
	const float kFontHigth=1.5f;
	const float kFontLocFront[3]{ 0.0f,0.0f,1.0f };
	const float kFontLocUp[3]{ 0.0f,1.0f,0.0f };

	const float kFontTextureCellWidth = 26.0f;
	const float kFontTextureCellHeigth = 32.0f;
	const float kFontTextureWidth = 256;
	const float kFontTextureHeight = 512;
	const float kFontTextureRowCharNum = 9;
	const float kFontTextureColumnCharNum = 11;
	const char kFontTextureStartChar = 32;//空格

	Texture LoadFontTexture();//仅用于initGL函数中
	OpenglObj::OpenglPointMsg LoadFontPoint(const std::string& text, const glm::vec3& position, float size);//相对位置
}