#pragma once
#include <cfloat>
#include <vector>
#include <memory>

#include "DxfPoint.h"
#include "DxfIPolygon.h"

namespace DxfReader {
	class Line :public IPolygon
	{
	public:
		Line(const Point& start_point, const Point& end_point)
			:start_point(start_point), end_point(end_point)
		{}
		Line(double x1,double y1,double x2,double y2)
			:start_point(x1,y1),end_point(x2,y2)
		{}
		std::string GetType() const override;
		void ShowType() const override;
		std::shared_ptr<IPolygon> clone() const override;
		void DownStartPoint(double dy) override;
		void DownEndPoint(double dy) override;
		void Show() const;
		double GetY(double x) const override;//标准轮专用
		std::vector<glm::vec2> GetUniformPoints(Direction direction,unsigned int section=0) const override;//获取均匀点集
		Point GetStartPoint() const override;
		Point GetEndPoint() const override;
	private:
		Point start_point;
		Point end_point;
	};
}