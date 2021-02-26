#include "LoadModel.h"
#include <iostream>

#include <qopenglfunctions_3_3_compatibility.h>

#include "stb_image.h"
void LoadModel::ShowMsg(std::vector<OpenglObj::Mesh> & meshes)
{
	//for (unsigned int i = 0; i != meshes.size(); ++i)
	//{
	//	//std::cout << "mesh" << i << ":" << "mesh VAO:" << meshes[i].VAO << std::endl;

	//	std::cout << "mesh" << i << ":" << "vertices size:"<<meshes[i].vertices.size() << std::endl;
	//	for (unsigned int j = 0; j != meshes[i].vertices.size(); ++j)
	//	{
	//		if (j < 20)
	//		{
	//			std::cout << meshes[i].vertices[j].Position.x << " " << meshes[i].vertices[j].Position.y << " " << meshes[i].vertices[j].Position.z << std::endl;
	//		}
	//	}
	//}

	//for (unsigned int i = 0; i != meshes.size(); ++i)
	//{
	//	//int i = 0;
	//	std::cout << "mesh" << i << ":" << "indices size:" << meshes[i].indices.size() << std::endl;
	//	for (unsigned int j = 0; j != meshes[i].indices.size(); ++j)
	//	{
	//		//if (j < 20)
	//		//{
	//			std::cout << meshes[i].indices[j] << std::endl;
	//		//}
	//	}
	//}
}
std::vector<OpenglObj::Mesh> LoadModel::loadModel(std::string path)
{
	std::vector<OpenglObj::Mesh> meshes;//һ��ģ�Ϳ����ж������

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//aiProcess_Triangulate ���ģ�Ͳ��ǣ�ȫ��������������ɣ�����Ҫ��ģ�����е�ͼԪ��״�任Ϊ������
	//aiProcess_FlipUVs���ڴ����ʱ��תy�����������
	//aiProcess_GenNormals�����ģ�Ͳ������������Ļ�����Ϊÿ�����㴴�����ߡ�
	//aiProcess_SplitLargeMeshes�����Ƚϴ������ָ�ɸ�С����������������Ⱦ����󶥵������ƣ�ֻ����Ⱦ��С��������ô����ǳ����á�
	//aiProcess_OptimizeMeshes�����ϸ�ѡ���෴�����Ὣ���С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ��ôӶ������Ż���
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//���ǻ��鳡��������ڵ㲻Ϊnull�����Ҽ��������һ�����(Flag)�����鿴���ص������ǲ��ǲ�������
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return meshes;
	}
	directory = path.substr(0, path.find_last_of('/'));//Ŀ¼
	processNode(scene->mRootNode, scene, meshes);

	this->textures_loaded.clear();
	//ShowMsg(meshes);
	return meshes;
}
void LoadModel::processNode(aiNode *node, const aiScene *scene, std::vector<OpenglObj::Mesh>& meshes)
{
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, meshes);
	}
}
OpenglObj::Mesh LoadModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
	//std::cout << "ok1.1" << std::endl;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//std::cout << "ok1.11" << std::endl;
		// ������λ�á����ߺ���������
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		//std::cout << "ok1.111" << std::endl;
		if (mesh->mNormals)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		else
		{
			std::cout << "WARNING::NO NORMALS TO BE LOAD!" << std::endl;
		}
		//std::cout << "ok1.12" << std::endl;
		if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		//std::cout << "ok1.13" << std::endl;
		//std::cout << "ok1.1" << std::endl;
		//if (mesh->mTangents)
		//{// tangent
		//	vector.x = mesh->mTangents[i].x;
		//	vector.y = mesh->mTangents[i].y;
		//	vector.z = mesh->mTangents[i].z;
		//	vertex.Tangent = vector;
		//}

		//std::cout << "ok1.2" << std::endl;
		//if (mesh->mBitangents)
		//{// bitangent
		//	vector.x = mesh->mBitangents[i].x;
		//	vector.y = mesh->mBitangents[i].y;
		//	vector.z = mesh->mBitangents[i].z;
		//	vertex.Bitangent = vector;
		//}
		//std::cout << "ok1.3" << std::endl;
		vertices.push_back(vertex);

	}

	//std::cout << "ok1.2" << std::endl;
	// ��������
	//std::cout << "mesh->mNumFaces:" << mesh->mNumFaces << std::endl;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];//�����
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	//std::cout << "ok1.3" << std::endl;
	// �������
	if (mesh->mMaterialIndex >= 0)//���һ�������Ƿ�����в��ʣ�mesh�Ĳ���idָ��scence�ϵĲ������飬ӦΪ���ʲ�һ����ֵ��һ��mesh��
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,  //��ȡ�����������ͼ
			aiTextureType_DIFFUSE, "texture_diffuse");
		//std::cout <<""<<diffuseMaps.size() << std::endl;
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, //
			aiTextureType_SPECULAR, "texture_specular");                   //��ȡ���������ͼ
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, 
			aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, 
			aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return OpenglObj::Mesh(vertices,indices,textures);
}
std::vector<Texture> LoadModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
	std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);//strΪ�����ļ���λ�ã�Ĭ��Ϊlocal·�����п��ܴ洢���Ǿ���·��
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // �������û�б����أ��������
			Texture texture;
			texture.id = LoadTextureFromFile(str.C_Str(), directory);//���ز���,ÿ����һ�ξ����һ������
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture); // ��ӵ��Ѽ��ص�������
		}
	}
	return textures;
}

unsigned int LoadModel::LoadTextureFromFile(const char *localPath,const std::string& directory)
{
	//QOpenGLFunctions_3_3_Core glfun;   //��Ҫ�̳У���ֵ��������
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();

	std::string fileName = localPath;
	fileName = directory + '/' + fileName;

	unsigned int texture;
	glGenTextures(1, &texture);

	int width, height, nrComponents;
	//stbi_set_flip_vertically_on_load(true);//assimp�Ѿ��������Ƿ������
	//OpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ���,stb_image.h�ܹ���ͼ�����ʱ�������Ƿ�תy��
	unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
	//"W:/CarWheel/LearnOpenGL_Light/LearnOpenGL_Light/block.png"
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)//��¼�ĳ�Ա��
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		//glPixelStorei(GL_UNPACK_ALIGNMENT, nrComponents);
		glfun.glBindTexture(GL_TEXTURE_2D, texture);
		glfun.glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);//Ϊtexture��ͼ������																							 //	���һ��������������ͼ�����ݡ�
		glfun.glGenerateMipmap(GL_TEXTURE_2D);

		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture at path:"<< fileName << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
