#pragma once
#include <vector>

#include <qpainter.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Mark.h"
#include "Texture.h"
#include "OpenglPointMsg.h"
#include "ShaderProgram.h"
class ColorGradientStrip
{
public:
	static const int kMarkTextDisToStrip;
	static const int kStripImageWidth;
	static const int kStripImageHeight;
	static const int kStripImageLUPointX;
	static const int kStripImageLUPointY;

	ColorGradientStrip():lu_point(glm::vec2(0.0f,0.0f)),w(10),h(100), strip_mesh()
	{
		strip_texture.id = INT_MIN;
	}
	ColorGradientStrip(int lu_x, int lu_y, int w, int h) :lu_point(glm::vec2(lu_x, lu_y)),w(w),h(h),strip_mesh()
	{
		strip_texture.id = INT_MIN;
	}

	void Draw(QPainter &painter);
	void Draw(ShaderProgram& shader);
	void AddMark(const Coordinate::Mark& mark);
	void set_lu_point(int x, int y);
	void set_w(int w);
	void set_h(int h);
	void show() const;
	void LoadOpenglMesh(int screen_width, int screen_height,float size);
private:
	QImage LoadQImage();
	void LoadOpenglTexture(QImage& image);
	OpenglObj::OpenglPointMsg LoadStripPoint(int x, int y, float size, int screen_width, int screen_height);

	glm::vec2 lu_point;
	int w;
	int h;

	std::vector<Coordinate::Mark> marks;
	bool t;

	Texture strip_texture;
	OpenglObj::Mesh strip_mesh;
};

ColorGradientStrip CreateColorGradientStrip(int position_x, int position_y, int w, int h, int text_size, float start_mark, float end_mark, int section);