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
class LoadModel  //工具类
{
public:
	/*  函数   */
	LoadModel(bool gamma=false):gammaCorrection(gamma){}
	void ShowMsg(std::vector<OpenglObj::Mesh> & meshes);
	std::vector<OpenglObj::Mesh> loadModel(std::string path);
private:
	/*  模型数据  */
	std::string directory;
	std::vector<Texture> textures_loaded;//不是提取，而是检查相同的texture是否被重复加载，texture只携带opengl给的id，可以被重复使用在不同mesh上
	bool gammaCorrection;
	/*  函数   */
	void processNode(aiNode *node, const aiScene *scene,std::vector<OpenglObj::Mesh>& meshes);
	OpenglObj::Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);

	unsigned int LoadTextureFromFile(const char *localPath, const std::string& directory);
};
