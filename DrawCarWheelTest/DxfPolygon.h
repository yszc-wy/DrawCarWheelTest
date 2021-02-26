#pragma once
#include <memory>

#include "DxfIPolygon.h"
namespace DxfReader {
	class Polygon {//如果想使用这个变量，除了赋值必须初始化，不提供其他的赋值函数接口
	public:
		Polygon();//空的值除了赋值以外都不能用
		Polygon(const std::shared_ptr<IPolygon>& p_ipolygon, Direction direction);
		Polygon(const Polygon& polygon);
		Polygon& operator=(const Polygon& polygon);
		//下面也没有可以直接修改相关值的函数
		//Point get_start_point() const;
		//Point get_end_point() const;
		Direction get_direction() const;
		void set_direction(Direction direction);
		void ReverseDirection();
		const std::shared_ptr<IPolygon>& get_p_ipolygon() const;
		void Show() const;
		std::string Get_Log() const;
	private:
		//void LoadSEPoint();

		std::shared_ptr<IPolygon> p_ipolygon;
		//Point start_point;//这个单纯的预加载一下
		//Point end_point;

		Direction direction;
	};
}