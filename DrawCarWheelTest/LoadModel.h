//#ifndef LOADMODEL_H
//#define LOADMODEL_H
#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderProgram.h"
#include "Texture.h"
#include "Mesh.h"
class LoadModel  //������
{
public:
	/*  ����   */
	LoadModel(bool gamma=false):gammaCorrection(gamma){}
	void ShowMsg(std::vector<OpenglObj::Mesh> & meshes);
	std::vector<OpenglObj::Mesh> loadModel(std::string path);
private:
	/*  ģ������  */
	std::string directory;
	std::vector<Texture> textures_loaded;//������ȡ�����Ǽ����ͬ��texture�Ƿ��ظ����أ�textureֻЯ��opengl����id�����Ա��ظ�ʹ���ڲ�ͬmesh��
	bool gammaCorrection;
	/*  ����   */
	void processNode(aiNode *node, const aiScene *scene,std::vector<OpenglObj::Mesh>& meshes);
	OpenglObj::Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);

	unsigned int LoadTextureFromFile(const char *localPath, const std::string& directory);
};
