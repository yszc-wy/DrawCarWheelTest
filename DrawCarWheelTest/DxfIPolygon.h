#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "DxfPoint.h"
namespace DxfReader {
	const double kPIR = 3.141592653;
	enum Direction { Forward, Reverse };//正方向，反方向
	class IPolygon
	{
	public:
		virtual std::shared_ptr<IPolygon> clone() const = 0;
		virtual double GetY(double x) const = 0;//标准轮专用
		virtual std::vector<glm::vec2> GetUniformPoints(Direction direction, unsigned int section = 0) const = 0;//获取均匀点集
		virtual Point GetStartPoint() const = 0;
		virtual Point GetEndPoint() const = 0;
		virtual void DownStartPoint(double dy) = 0;
		virtual void DownEndPoint(double dy) = 0;
		virtual void ShowType() const= 0;
		virtual std::string GetType() const =0;
		virtual ~IPolygon() = default;
	};
}