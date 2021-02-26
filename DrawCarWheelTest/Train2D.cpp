#include "Train2D.h"
#include <iostream>
const int Train2D::kBoxLength=60;
const int Train2D::kBoxWidth=15;
const int Train2D::kBoxGap= kBoxWidth/2.0f;
const int Train2D::kTopOfBoxHeadLength = kBoxLength/6.0f;
const int Train2D::kTopOfBoxHeadWidth = kBoxWidth/1.3f;
const int Train2D::kTextSize = 10;
const int Train2D::kTextOffset = -10;
void Train2D::set_marshalling(int marshalling)
{
	this->marshalling = marshalling;
}
int Train2D::PickBox(int x, int y)
{
	if (this->marshalling < 2)
	{
		std::cout << "Error::Train2D::PickBox::illegal marshalling" << std::endl;
		return -1;
	}
	for (int i = 0; i != this->marshalling; ++i)
	{
		if (x >= this->lu_points[i].x&&x <= this->lu_points[i].x + kBoxWidth&&y>=this->lu_points[i].y&&y<=lu_points[i].y+kBoxLength)
		{
			return i;
		}
	}
	return -1;
}
void Train2D::set_lu_point(int x, int y)
{
	this->lu_point = glm::vec2(x, y);
}
void Train2D::BuildLUPoints()
{
	if (this->marshalling < 2)
	{
		std::cout << "Error::Train2D::BuildLUPoints::illegal marshalling" << std::endl;
		return ;
	}
	//int start_point_y = this->ld_point.y-(this->marshalling*kBoxLength + (this->marshalling - 1)*kBoxGap);
	for (int i = 0; i != this->marshalling; ++i)
	{
		glm::vec2 point;
		point= glm::vec2(this->lu_point.x, this->lu_point.y + (kBoxLength + kBoxGap)*i);
		this->lu_points.push_back(point);
	}
}
void Train2D::Draw(QPainter &painter)
{
	if (this->marshalling < 2)
	{
		std::cout << "Error::Train2D::Draw::illegal marshalling" << std::endl;
		return;
	}
	if (this->lu_points.size() == 0)
	{
		std::cout << "Error::Train2D::Draw::lu_points.size()=0" << std::endl;
		return;
	}
	painter.setRenderHint(QPainter::Antialiasing, true);
	for (int i = 0; i != this->marshalling; ++i)
	{
		if (i == 0)
		{
			this->DrawTrainHead(this->lu_points[i].x, this->lu_points[i].y, i, painter);
		}
		else
		if (i == this->marshalling - 1)
		{
			this->DrawTrainTail(this->lu_points[i].x, this->lu_points[i].y, i, painter);
		}
		else
		this->DrawTrainBox(this->lu_points[i].x, this->lu_points[i].y, i, painter);
	}
}

void Train2D::DrawTrainBox(int x, int y, int id, QPainter &painter)
{
	QBrush brush(QColor(135, 206, 250));//创建画刷  
	painter.setBrush(brush);                          //使用画刷  
	painter.drawRect(x, y, kBoxWidth, kBoxLength);

	QPen pen(Qt::white, 1);
	painter.setPen(pen); //使用画笔  
	painter.drawRect(x, y, kBoxWidth, kBoxLength);

	//text
	this->DrawText(x, y, id, painter);
}

void Train2D::DrawTrainHead(int x, int y, int id, QPainter &painter)
{
	QBrush brush(QColor(135, 206, 250));//创建画刷  
	painter.setBrush(brush);                          //使用画刷  
	static const QPoint points[6] = {
		QPoint(x + (kBoxWidth-kTopOfBoxHeadWidth) / 2.0f, y),
		QPoint(x + kTopOfBoxHeadWidth + (kBoxWidth-kTopOfBoxHeadWidth) / 2.0f, y),
		QPoint(x + kBoxWidth, y + kTopOfBoxHeadLength),
		QPoint(x + kBoxWidth, y + kBoxLength),
		QPoint(x, y + kBoxLength),
		QPoint(x, y + kTopOfBoxHeadLength)
	};

	//static const QPoint points[6] = {
	//	QPoint(20,30),
	//	QPoint(40,50),
	//	QPoint(60,100),
	//	QPoint(80,120),
	//	QPoint(100,190),
	//	QPoint(300,250)
	//};

	painter.drawPolygon(points,6);

	QPen pen(Qt::white, 1);
	painter.setPen(pen); //使用画笔  
	painter.drawPolygon(points, 6);

	//text
	this->DrawText(x, y, id, painter);
}

void Train2D::DrawTrainTail(int x, int y, int id, QPainter &painter)
{
	QBrush brush(QColor(135, 206, 250));//创建画刷  
	painter.setBrush(brush);                          //使用画刷  
	static const QPoint points[6] = {
		QPoint(x + (kBoxWidth - kTopOfBoxHeadWidth) / 2.0f, y+kBoxLength),
		QPoint(x + kTopOfBoxHeadWidth + (kBoxWidth - kTopOfBoxHeadWidth) / 2.0f, y + kBoxLength),
		QPoint(x + kBoxWidth, y + kBoxLength - kTopOfBoxHeadLength),
		QPoint(x + kBoxWidth, y),
		QPoint(x, y),
		QPoint(x, y + kBoxLength - kTopOfBoxHeadLength)
	};
	painter.drawPolygon(points, 6);

	QPen pen(Qt::white, 1);
	painter.setPen(pen); //使用画笔  
	painter.drawPolygon(points, 6);

	this->DrawText(x, y, id, painter);
}

void Train2D::DrawText(int x,int y,int id, QPainter &painter)
{
	//text
	painter.setPen(Qt::red);
	painter.setFont(QFont("Arial", kTextSize,QFont::Bold));

	painter.drawText(x, y + kBoxLength / 2.0f + kTextOffset, kBoxWidth, kBoxWidth, Qt::AlignCenter, std::to_string(id).c_str());
}