#pragma once
#include <string>
#include <assimp/Importer.hpp>
struct Texture {
	unsigned int id;//�洢�Ѿ����غõ�texture id
	std::string type;//��ʲô����Ĳ���
	aiString path;
};