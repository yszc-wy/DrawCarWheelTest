#include "Font.h"
#include <iostream>

//#include <qopenglfunctions_3_3_core.h>
#include <qopenglfunctions_3_3_compatibility.h>
#include "stb_image.h"
#include "FilePath.h"
namespace Font {
	std::vector<glm::vec2> LoadTexCoords(char c)
	{
		int row_id = (c - kFontTextureStartChar) % 9;
		int column_id = (c - kFontTextureStartChar) / 9;

		float unit_x = kFontTextureCellWidth / kFontTextureWidth;
		float unit_y = kFontTextureCellHeigth / kFontTextureHeight;

		float start_x = row_id*unit_x;
		float start_y = 1.0f - column_id*unit_y;

		glm::vec2 up_left = glm::vec2(start_x, start_y);
		glm::vec2 up_right = glm::vec2(start_x + unit_x, start_y);
		glm::vec2 down_left = glm::vec2(start_x, start_y - unit_y);
		glm::vec2 down_right = glm::vec2(start_x + unit_x, start_y - unit_y);

		return std::vector<glm::vec2>{up_left, up_right, down_left, down_right};
	}

	Texture LoadFontTexture()
	{

		//QOpenGLFunctions_3_3_Core glfun;   //��Ҫ�̳У���ֵ��������
		QOpenGLFunctions_3_3_Compatibility glfun;
		glfun.initializeOpenGLFunctions();

		unsigned int font_texture;
		glfun.glGenTextures(1, &font_texture);
		glfun.glBindTexture(GL_TEXTURE_2D, font_texture);//������,��������
												   //����������Ľ��ͷǳ����ɣ������Բ��ü��ֲ�ͬ�Ĳ�ֵ��ʽ������������Ҫ�Լ�����OpenGL�����������������
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//�趨2D�����x�᷽���ϵĻ��Ʒ�ʽ
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//y�᷽���ϵĻ��Ʒ�ʽ
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//���˷�ʽ�����Թ���
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//OpenGL��Ҫ֪����������������(Texture Pixel��Ҳ��Texel����ע1)ӳ�䵽�������꣬��Ϊ�������
		//�ͷֱ���ͼ��ӳ�䵽�������ϣ�һ�����������ϻ��ж����������ӳ�䣬����GL_NEAREST�������ػ��������GL_LINEAR��ģ��������ƽ��
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		//OpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ���,stb_image.h�ܹ���ͼ�����ʱ�������Ƿ�תy��
		unsigned char *data = stbi_load(FilePath::kFontTexturePath, &width, &height, &nrChannels, 0);
		if (data)
		{
			//���Ƶ�������ȡ���ݵĶ��뷽ʽ��Ĭ��4�ֽڶ��룬��һ�е�ͼ�������ֽ���������4����������
			//����ȡ����ʱ����ȡ4���ֽ�������Ⱦһ�У�֮���ȡ4�ֽ�����������Ⱦ�ڶ��С�
			//��RGB 3�ֽ����ض��ԣ���һ��10�����أ���30���ֽڣ���4�ֽڶ���ģʽ�£�
			//OpenGL���ȡ32���ֽڵ����ݣ�������ע�⣬�ᵼ��glTextImage���º����Ķ�ȡԽ�磬
			//�Ӷ�ȫ�����.
			glfun.glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glfun.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//Ϊtexture��ͼ������																							 
			//  ��һ������ָ��������Ŀ��(Target)������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ������κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
			//	���һ��������������ͼ�����ݡ�
			
			glfun.glGenerateMipmap(GL_TEXTURE_2D);
			//������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��Ȼ����Ŀǰֻ�л�������(Base - level)������ͼ�񱻼����ˣ�
			//���Ҫʹ�ö༶��Զ�������Ǳ����ֶ��������в�ͬ��ͼ�񣨲��ϵ����ڶ��������������ߣ�ֱ������������֮�����glGenerateMipmap��
			//���Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);//�󶨺�Ϳ����ͷ��ڴ���

		Texture output_texture;
		output_texture.id = font_texture;
		output_texture.type = "texture_diffuse";
		output_texture.path = "";
		return output_texture;
		//ע�⣡�����Դ��δ�ͷŵģ����ܵ�����Դй¶
	}

	OpenglObj::OpenglPointMsg LoadFontPoint(const std::string& text, const glm::vec3& position, float size)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		int text_size=0;
		for (auto i : text)
		{
			if (i == '\n')
			{
				break;
			}
			++text_size;
		}
		//auto text_size = text.size();

		glm::vec3 origin_position(0.0f, 0.0f, 0.0f);
		origin_position.x = position.x - kFontWidth*size*text_size / 2.0f;
		origin_position.y = position.y + kFontHigth*size / 2.0f;
		origin_position.z = position.z;

		//origin_position.x = position.x;
		//origin_position.y = position.y;
		//origin_position.z = position.z;

		text_size= text.size();
		float row_count = 0;
		float col_count = 0;
		for (int i = 0; i != text_size; ++i)
		{
			if (text[i] == '\n')
			{
				++row_count;
				col_count = 0;//������0
				continue;
			}
			Vertex up_left;
			up_left.Position = glm::vec3(origin_position.x + col_count*kFontWidth*size, origin_position.y- kFontHigth*size*row_count, origin_position.z);
			
			Vertex up_right;
			up_right.Position = glm::vec3(origin_position.x + (col_count +1)*kFontWidth*size, origin_position.y- kFontHigth*size*row_count, origin_position.z);

			Vertex down_left;
			down_left.Position = glm::vec3(origin_position.x + col_count*kFontWidth*size, origin_position.y-kFontHigth*size- kFontHigth*size*row_count, origin_position.z);

			Vertex down_right;
			down_right.Position = glm::vec3(origin_position.x + (col_count +1)*kFontWidth*size, origin_position.y-kFontHigth*size - kFontHigth*size*row_count, origin_position.z);

			++col_count;//������

			std::vector<glm::vec2> tex_coords = LoadTexCoords(text[i]);
			up_left.TexCoords = tex_coords[0];
			up_right.TexCoords = tex_coords[1];
			down_left.TexCoords= tex_coords[2];
			down_right.TexCoords= tex_coords[3];

			int start_vertice_id = vertices.size();
			vertices.push_back(up_left);
			vertices.push_back(up_right);
			vertices.push_back(down_left);
			vertices.push_back(down_right);

			indices.push_back(start_vertice_id);
			indices.push_back(start_vertice_id+2);
			indices.push_back(start_vertice_id+1);

			indices.push_back(start_vertice_id+1);
			indices.push_back(start_vertice_id+2);
			indices.push_back(start_vertice_id+3);
		}

		return OpenglObj::OpenglPointMsg(std::move(vertices), std::move(indices));
	}
	
	
}