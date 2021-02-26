#pragma once
#include <cfloat>
#include <cmath>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "DxfIPolygon.h"
#include "DxfPoint.h"
namespace DxfReader {

	class Circle :public IPolygon
	{
	public:
		Circle(const Point& center_point, double r, double start_angle, double end_angle)
			:center_point(center_point), r(r), start_angle(start_angle), end_angle(end_angle)
		{}
		Circle(double cx, double cy, double r, double start_angle, double end_angle)
			:center_point(cx,cy), r(r), start_angle(start_angle), end_angle(end_angle)
		{}
		std::string GetType() const override;
		void ShowType() const override;
		void DownStartPoint(double dy) override;
		void DownEndPoint(double dy) override;
		std::shared_ptr<IPolygon> clone() const override;

		double GetY(double x) const override;//��׼��ר��
		std::vector<glm::vec2> GetUniformPoints(Direction direction, unsigned int section=0) const override;//��ȡ���ȵ㼯
		Point GetStartPoint() const override;
		Point GetEndPoint() const override;
	private:
		bool InAngleRange(double angle) const;

		Point center_point;
		double r;
		double start_angle;
		double end_angle;
	};
	double ConvertToRadian(double angle);
	double ConvertToStdAngle(double angle);//0-360
	double ConvertToAngle(double radian);//����ת��Ϊangle
	double GetPointAngleToBasePoint(double x, double y);//��ȡ�㵽ԭ��ĽǶȣ�0-360
	Point GetPointFromPolar(double radian, double r,double m_x=0.0f,double m_y=0.0f);
}