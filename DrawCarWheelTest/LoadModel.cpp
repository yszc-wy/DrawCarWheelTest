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
	std::vector<OpenglObj::Mesh> meshes;//一个模型可能有多个网格

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//aiProcess_Triangulate 如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形
	//aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标
	//aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
	//aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用。
	//aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//我们会检查场景和其根节点不为null，并且检查了它的一个标记(Flag)，来查看返回的数据是不是不完整的
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return meshes;
	}
	directory = path.substr(0, path.find_last_of('/'));//目录
	processNode(scene->mRootNode, scene, meshes);

	this->textures_loaded.clear();
	//ShowMsg(meshes);
	return meshes;
}
void LoadModel::processNode(aiNode *node, const aiScene *scene, std::vector<OpenglObj::Mesh>& meshes)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// 接下来对它的子节点重复这一过程
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
		// 处理顶点位置、法线和纹理坐标
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
		if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
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
	// 处理索引
	//std::cout << "mesh->mNumFaces:" << mesh->mNumFaces << std::endl;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];//获得面
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	//std::cout << "ok1.3" << std::endl;
	// 处理材质
	if (mesh->mMaterialIndex >= 0)//检测一个网格是否包含有材质，mesh的材质id指向scence上的材质数组，应为材质不一定赋值在一个mesh上
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,  //获取多个漫反射贴图
			aiTextureType_DIFFUSE, "texture_diffuse");
		//std::cout <<""<<diffuseMaps.size() << std::endl;
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, //
			aiTextureType_SPECULAR, "texture_specular");                   //获取多个镜面贴图
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
		mat->GetTexture(type, i, &str);//str为纹理文件的位置，默认为local路径，有可能存储的是绝对路径
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
		{   // 如果纹理还没有被加载，则加载它
			Texture texture;
			texture.id = LoadTextureFromFile(str.C_Str(), directory);//加载材质,每运行一次就添加一次数据
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture); // 添加到已加载的纹理中
		}
	}
	return textures;
}

unsigned int LoadModel::LoadTextureFromFile(const char *localPath,const std::string& directory)
{
	//QOpenGLFunctions_3_3_Core glfun;   //不要继承，赋值会有问题
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();

	std::string fileName = localPath;
	fileName = directory + '/' + fileName;

	unsigned int texture;
	glGenTextures(1, &texture);

	int width, height, nrComponents;
	//stbi_set_flip_vertically_on_load(true);//assimp已经帮助我们反向过了
	//OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部,stb_image.h能够在图像加载时帮助我们翻转y轴
	unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
	//"W:/CarWheel/LearnOpenGL_Light/LearnOpenGL_Light/block.png"
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)//记录的成员数
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		//glPixelStorei(GL_UNPACK_ALIGNMENT, nrComponents);
		glfun.glBindTexture(GL_TEXTURE_2D, texture);
		glfun.glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);//为texture绑定图像数据																							 //	最后一个参数是真正的图像数据。
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
