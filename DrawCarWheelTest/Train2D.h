#pragma once
#include <vector>

#include <glm/glm.hpp>
#include <qpainter.h>

class Train2D
{
public:
	static const int kBoxLength;
	static const int kBoxWidth;
	static const int kBoxGap;
	static const int kTopOfBoxHeadLength;
	static const int kTopOfBoxHeadWidth;
	static const int kTextSize;
	static const int kTextOffset;

	Train2D():lu_point(glm::vec2(0.0f,0.0f)),marshalling(3){}

	void set_marshalling(int marshalling);
	void set_lu_point(int x, int y);
	void BuildLUPoints();
	void Draw(QPainter &painter);
	int PickBox(int x, int y);
private:

	void DrawTrainBox(int x, int y, int id, QPainter &painter);
	void DrawTrainHead(int x, int y, int id, QPainter &painter);
	void DrawTrainTail(int x, int y, int id, QPainter &painter);
	void DrawText(int x, int y, int id, QPainter &painter);

	glm::vec2 lu_point;
	int marshalling;

	std::vector<glm::vec2> lu_points;
};