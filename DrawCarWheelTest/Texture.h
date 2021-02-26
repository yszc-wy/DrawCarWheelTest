#pragma once
#include <string>
#include <assimp/Importer.hpp>
struct Texture {
	unsigned int id;//存储已经加载好的texture id
	std::string type;//是什么种类的材质
	aiString path;
};