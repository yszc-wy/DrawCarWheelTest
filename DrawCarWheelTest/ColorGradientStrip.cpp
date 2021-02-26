#include "ColorGradientStrip.h"
#include <iostream>
//#include <qopenglfunctions_3_3_core.h>
#include <qopenglfunctions_3_3_compatibility.h>

#include "Mesh.h"

const int ColorGradientStrip::kMarkTextDisToStrip=1;
const int ColorGradientStrip::kStripImageWidth=100;
const int ColorGradientStrip::kStripImageHeight=300;
const int ColorGradientStrip::kStripImageLUPointX=10;
const int ColorGradientStrip::kStripImageLUPointY=20;
void ColorGradientStrip::show() const
{
	std::cout << "ColorGradientStrip::show: lupoint.x:" << this->lu_point.x<<" lupoint.y:"<<this->lu_point.y<<" w:"<<this->w<<" h:"<<this->h << std::endl;
}
void ColorGradientStrip::LoadOpenglMesh(int screen_width, int screen_height,float size)
{
	//load qimage
	//std::cout << "ColorGradientStrip::LoadOpenglMesh::start" << std::endl;
	this->LoadOpenglTexture(this->LoadQImage());

	auto opengl_strip_points = this->LoadStripPoint(lu_point.x, lu_point.y, size, screen_width, screen_height);

	//std::cout << "\nColorGradientStrip::LoadOpenglMesh::Start" << std::endl;
	//std::cout << "ColorGradientStrip::LoadOpenglMesh::this->strip_mesh is_set_up" << strip_mesh.get_is_set_up() << std::endl;
	this->strip_mesh.DeleteMesh();
	//std::cout << "\nColorGradientStrip::LoadOpenglMesh::End" << std::endl;

	this->strip_mesh = OpenglObj::Mesh(opengl_strip_points, std::vector<Texture>{this->strip_texture});
	//std::cout << "ColorGradientStrip::LoadOpenglMesh::this->strip_mesh is_set_up" << strip_mesh.get_is_set_up() << std::endl;
	//std::cout << "ColorGradientStrip::LoadOpenglMesh::end" << std::endl;
}
QImage ColorGradientStrip::LoadQImage()
{
	QImage stripImage(kStripImageWidth, kStripImageHeight, QImage::Format_RGB888);
	{
		QPainter painter(&stripImage);
		painter.fillRect(0, 0, kStripImageWidth, kStripImageHeight, QColor(1, 1, 1));

		int line_x = (kStripImageLUPointX + w / 2.0f) / 2.0f;
		QLinearGradient linear_gradient(line_x, kStripImageLUPointY + h, line_x, kStripImageLUPointY);//底部到高
		for (auto &mark : this->marks)
		{
			if (mark.position_ratio < 0 || mark.position_ratio>1)
			{
				std::cout << "ColorGradientStrip::Draw::illegal position_ratio" << std::endl;
				continue;
			}
			if (mark.line_color.x == -1.0f&&mark.line_color.y == -1.0f&&mark.line_color.z == -1.0f)//只加刻度不加颜色
			{
				continue;
			}
			if (mark.line_color.x < 0 || mark.line_color.x>1
				|| mark.line_color.y < 0 || mark.line_color.y>1
				|| mark.line_color.z < 0 || mark.line_color.z>1)
			{
				std::cout << "ColorGradientStrip::Draw::illegal color" << std::endl;
				continue;
			}
			glm::vec3 color = 255.0f*mark.line_color;
			linear_gradient.setColorAt(mark.position_ratio, QColor(color.x, color.y, color.z));
		}

		painter.setBrush(linear_gradient);
		painter.drawRect(kStripImageLUPointX, kStripImageLUPointY, w, h);

		//画文本
		for (auto &mark : this->marks)
		{
			//std::cout << "Draw Text" << std::endl;
			if (mark.text.get_text() == "")
			{
				continue;
			}

			glm::vec3 color = 255.0f*mark.text.get_color();
			painter.setPen(QColor(color.x, color.y, color.y));
			painter.setFont(QFont("Arial", mark.text.get_size()));

			glm::vec2 text_position = glm::vec2(kStripImageLUPointX + w + kMarkTextDisToStrip, kStripImageLUPointY + ((1 - mark.position_ratio)*h));
			painter.drawText(text_position.x, text_position.y, mark.text.get_text().c_str());
		}
	}

	return stripImage;
}
void ColorGradientStrip::Draw(ShaderProgram& shader)
{
	//QImage image;
	//QPainter painter(&image);

	shader.use();
	shader.setInt("strip_texture", 0);//GL_TEXTURE0
	this->strip_mesh.Draw(shader);
}
void ColorGradientStrip::LoadOpenglTexture(QImage& image)
{
	QOpenGLFunctions_3_3_Compatibility glfun;
	glfun.initializeOpenGLFunctions();

	if(this->strip_texture.id!= INT_MIN)//删除之前的纹理
		glfun.glDeleteTextures(1, &this->strip_texture.id);

	unsigned int strip_texture;
	glfun.glGenTextures(1, &strip_texture);
	glfun.glBindTexture(GL_TEXTURE_2D, strip_texture);//绑定纹理,方便配置
	//对纹理采样的解释非常宽松，它可以采用几种不同的插值方式。所以我们需要自己告诉OpenGL该怎样对纹理采样。
	glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//设定2D纹理的x轴方向上的环绕方式
	glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//y轴方向上的环绕方式
	glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//过滤方式，线性过滤
	glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_IMAGES, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGB8, kStripImageWidth, kStripImageHeight, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image.constBits());

	this->strip_texture.id = strip_texture;
	this->strip_texture.type = "texture_diffuse";
	this->strip_texture.path = "";



	//QOpenGLFunctions_3_3_Core glfun;   //不要继承，赋值会有问题
	//glfun.initializeOpenGLFunctions();

	//unsigned int font_texture;
	//glfun.glGenTextures(1, &font_texture);
	//glfun.glBindTexture(GL_TEXTURE_2D, font_texture);//绑定纹理,方便配置
	//												 //对纹理采样的解释非常宽松，它可以采用几种不同的插值方式。所以我们需要自己告诉OpenGL该怎样对纹理采样。
	//glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//设定2D纹理的x轴方向上的环绕方式
	//glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//y轴方向上的环绕方式
	//glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//过滤方式，线性过滤
	//glfun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	////OpenGL需要知道怎样将纹理像素(Texture Pixel，也叫Texel，译注1)映射到纹理坐标，即为纹理过滤
	////低分辨率图像映射到大物体上，一个纹理像素上会有多个纹理坐标映射，所以GL_NEAREST会有像素化的情况，GL_LINEAR会模糊化但很平滑
	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
}
OpenglObj::OpenglPointMsg ColorGradientStrip::LoadStripPoint(int x,int y,float size, int screen_width, int screen_height)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//origin_position.x = position.x;
	//origin_position.y = position.y;
	//origin_position.z = position.z;

	float opengl_x = 2 * x / (float)screen_width - 1;
	float opengl_y = -(2 * y / (float)screen_height - 1);
	//std::cout << "OpenglPointMsg ColorGradientStrip::Opengl_x:" << opengl_x << " opengl_y:" << opengl_y << std::endl;
	float opengl_width = 2*kStripImageWidth / (float)screen_width;
	float opengl_height = 2*kStripImageHeight / (float)screen_height;
	//std::cout << "OpenglPointMsg ColorGradientStrip::opengl_width:" << opengl_width << " opengl_height:" << opengl_height 
	//	<<" screen_width:"<< screen_width <<" screen_height:"<< screen_height << std::endl;

	Vertex up_left;
	up_left.Position = glm::vec3(opengl_x, opengl_y, 0.0f);

	Vertex up_right;
	up_right.Position = glm::vec3(opengl_x + opengl_width*size, opengl_y, 0.0f);

	Vertex down_left;
	down_left.Position = glm::vec3(opengl_x, opengl_y- opengl_height, 0.0f);

	Vertex down_right;
	down_right.Position = glm::vec3(opengl_x + opengl_width*size, opengl_y - opengl_height, 0.0f);

	up_left.TexCoords = glm::vec2(0.0f,1.0f-1.0f);
	up_right.TexCoords = glm::vec2(1.0f, 1.0f - 1.0f);
	down_left.TexCoords = glm::vec2(0.0f, 1.0f - 0.0f);
	down_right.TexCoords = glm::vec2(1.0f, 1.0f - 0.0f);

	int start_vertice_id = vertices.size();
	vertices.push_back(up_left);
	vertices.push_back(up_right);
	vertices.push_back(down_left);
	vertices.push_back(down_right);

	indices.push_back(start_vertice_id);
	indices.push_back(start_vertice_id + 2);
	indices.push_back(start_vertice_id + 1);

	indices.push_back(start_vertice_id + 1);
	indices.push_back(start_vertice_id + 2);
	indices.push_back(start_vertice_id + 3);
	

	return OpenglObj::OpenglPointMsg(std::move(vertices), std::move(indices));
}

void ColorGradientStrip::Draw(QPainter &painter)
{
	//std::cout << "ColorGradientStrip::Draw" << std::endl;
	//画颜色条

	int line_x = (this->lu_point.x + w/2.0f) / 2.0f;
	QLinearGradient linear_gradient(line_x, this->lu_point.y + h, line_x, this->lu_point.y);//底部到高
	for (auto &mark :this->marks)
	{
		if (mark.position_ratio < 0 || mark.position_ratio>1)
		{
			std::cout << "ColorGradientStrip::Draw::illegal position_ratio" << std::endl;
			continue;
		}
		if (mark.line_color.x == -1.0f&&mark.line_color.y == -1.0f&&mark.line_color.z == -1.0f)//只加刻度不加颜色
		{
			continue;
		}
		if (mark.line_color.x < 0 || mark.line_color.x>1
			|| mark.line_color.y < 0 || mark.line_color.y>1
			|| mark.line_color.z < 0 || mark.line_color.z>1)
		{
			std::cout << "ColorGradientStrip::Draw::illegal color" << std::endl;
			continue;
		}
		glm::vec3 color = 255.0f*mark.line_color;
		linear_gradient.setColorAt(mark.position_ratio, QColor(color.x, color.y, color.z));
	}

	painter.setBrush(linear_gradient);
	painter.drawRect(this->lu_point.x, this->lu_point.y, w, h);

	//if (this->t)
	//{
	//	t = false;
	//	painter.drawText(100, 100, "fuck");
	//}
	//else
	//{
	//	t = true;
	//}

	//
	//
	//glm::vec3 color = glm::vec3(255, 255, 255);
	//painter.setPen(QColor(color.x, color.y, color.y));
	//painter.setFont(QFont("Arial", /*mark.text.get_size()*/15));
	//for (auto &mark : this->marks)
	//{
	//	//glm::vec3 color = glm::vec3(255, 255, 255);
	//	//painter.setPen(QColor(color.x, color.y, color.y));
	//	//painter.setFont(QFont("Arial", /*mark.text.get_size()*/15));

	//	painter.drawText(100, 100, "fuck");
	//	//painter.drawPoint(100, 100);
	//}

	//glm::vec3 color = glm::vec3(255,255,255);
	//painter.setPen(QColor(color.x, color.y, color.y));
	//painter.setFont(QFont("Arial",14));

	//画文本
	for (auto &mark : this->marks)
	{
		//std::cout << "Draw Text" << std::endl;
		if (mark.text.get_text() == "")
		{
			continue;
		}

		glm::vec3 color = 255.0f*mark.text.get_color();
		painter.setPen(QColor(color.x,color.y,color.y));
		painter.setFont(QFont("Arial", mark.text.get_size()));

		glm::vec2 text_position=glm::vec2(lu_point.x+w+kMarkTextDisToStrip, lu_point.y+((1-mark.position_ratio)*h));
		painter.drawText(text_position.x,text_position.y, mark.text.get_text().c_str());

		//glm::vec3 color = glm::vec3(255, 255, 255);
		//painter.setPen(QColor(color.x, color.y, color.y));
		//painter.setFont(QFont("Arial", /*mark.text.get_size()*/15));
		//painter.drawText(100,100, "fuck");
	}
}
void ColorGradientStrip::AddMark(const Coordinate::Mark& mark)
{
	if (mark.position_ratio < 0 || mark.position_ratio>1)
	{
		std::cout << "ColorGradientStrip::AddMark::illegal position_ratio" << std::endl;
		return;
	}
	if ((mark.line_color.x < 0 || mark.line_color.x>1
		|| mark.line_color.y < 0 || mark.line_color.y>1
		|| mark.line_color.z < 0 || mark.line_color.z>1)&&
		!(mark.line_color.x == -1.0f&&mark.line_color.y == -1.0f&&mark.line_color.z == -1.0f))
	{
		std::cout << "ColorGradientStrip::AddMark::illegal color" << std::endl;
		return;
	}
	this->marks.push_back(mark);
}
void ColorGradientStrip::set_lu_point(int x, int y)
{
	this->lu_point = glm::vec2(x, y);
}
void ColorGradientStrip::set_w(int w)
{
	this->w = w;
}
void ColorGradientStrip::set_h(int h)
{
	this->h = h;
}

ColorGradientStrip CreateColorGradientStrip(int position_x,int position_y,int w,int h,int text_size,float start_mark, float end_mark, int section)
{
	ColorGradientStrip color_gradient_strip;
	color_gradient_strip.set_lu_point(position_x, position_y);
	color_gradient_strip.set_w(w);
	color_gradient_strip.set_h(h);

	color_gradient_strip.AddMark(Coordinate::Mark(0.0f / 4.0f, glm::vec3(0.0, 0.0, 1.0), Text("", text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	color_gradient_strip.AddMark(Coordinate::Mark(1.0f / 4.0f, glm::vec3(0.0, 1.0, 1.0), Text("", text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	color_gradient_strip.AddMark(Coordinate::Mark(2.0f / 4.0f, glm::vec3(0.0, 1.0, 0.0), Text("", text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	color_gradient_strip.AddMark(Coordinate::Mark(3.0f / 4.0f, glm::vec3(1.0, 1.0, 0.0), Text("", text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	color_gradient_strip.AddMark(Coordinate::Mark(4.0f / 4.0f, glm::vec3(1.0, 0.0, 0.0), Text("", text_size, glm::vec3(1.0f, 1.0f, 1.0f))));


	if (section < 0)
	{
		std::cout << "Error::CreateColorGradientStrip::illegal section" << std::endl;
		return ColorGradientStrip();
	}

	if (section == 0)
	{
		return color_gradient_strip;
	}

	float d_mark = end_mark - start_mark;
	float unit_position_ratio = 1.0f / (float)section;
	for (int i = 0; i != section + 1; ++i)
	{
		float mark_position_ratio = unit_position_ratio*i;
		float mark_text= start_mark+d_mark*mark_position_ratio;

		char str[50];
		std::sprintf(str, "%.2f", mark_text);
		color_gradient_strip.AddMark(Coordinate::Mark(mark_position_ratio, glm::vec3(-1.0, -1.0, -1.0), Text(std::string(str)+"mm", text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	}

	return color_gradient_strip;
}