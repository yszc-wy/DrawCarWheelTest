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

		//QOpenGLFunctions_3_3_Core glfun;   //不要继承，赋值会有问题
		QOpenGLFunctions_3_3_Compatibility glfun;
		glfun.initializeOpenGLFunctions();

		unsigned int font_texture;
		glfun.glGenTextures(1, &font_texture);
		glfun.glBindTexture(GL_TEXTURE_2D, font_texture);//绑定纹理,方便配置
												   //对纹理采样的解释非常宽松，它可以采用几种不同的插值方式。所以我们需要自己告诉OpenGL该怎样对纹理采样。
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//设定2D纹理的x轴方向上的环绕方式
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//y轴方向上的环绕方式
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//过滤方式，线性过滤
		glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//OpenGL需要知道怎样将纹理像素(Texture Pixel，也叫Texel，译注1)映射到纹理坐标，即为纹理过滤
		//低分辨率图像映射到大物体上，一个纹理像素上会有多个纹理坐标映射，所以GL_NEAREST会有像素化的情况，GL_LINEAR会模糊化但很平滑
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		//OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部,stb_image.h能够在图像加载时帮助我们翻转y轴
		unsigned char *data = stbi_load(FilePath::kFontTexturePath, &width, &height, &nrChannels, 0);
		if (data)
		{
			//控制的是所读取数据的对齐方式，默认4字节对齐，即一行的图像数据字节数必须是4的整数倍，
			//即读取数据时，读取4个字节用来渲染一行，之后读取4字节数据用来渲染第二行。
			//对RGB 3字节像素而言，若一行10个像素，即30个字节，在4字节对齐模式下，
			//OpenGL会读取32个字节的数据，若不加注意，会导致glTextImage中致函数的读取越界，
			//从而全面崩溃.
			glfun.glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glfun.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//为texture绑定图像数据																							 
			//  第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
			//	最后一个参数是真正的图像数据。
			
			glfun.glGenerateMipmap(GL_TEXTURE_2D);
			//当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像。然而，目前只有基本级别(Base - level)的纹理图像被加载了，
			//如果要使用多级渐远纹理，我们必须手动设置所有不同的图像（不断递增第二个参数）。或者，直接在生成纹理之后调用glGenerateMipmap。
			//这会为当前绑定的纹理自动生成所有需要的多级渐远纹理。
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);//绑定后就可以释放内存了

		Texture output_texture;
		output_texture.id = font_texture;
		output_texture.type = "texture_diffuse";
		output_texture.path = "";
		return output_texture;
		//注意！这个资源是未释放的，可能导致资源泄露
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
				col_count = 0;//行数归0
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

			++col_count;//行数加

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