#pragma once
#include <vector>
#include <memory>

#include "DxfPolygon.h"
namespace DxfReader {
	class StandardFlangeTreadMsgMaker
	{
	public:
		static const double kStandardTreadPointX;
		StandardFlangeTreadMsgMaker():cad_tread_height(0),cad_x_start(0),cad_x_end(0), standard_flange_tread_msg(){}
		StandardFlangeTreadMsgMaker(const std::vector<Polygon>& standard_tread_msg);
		double GetHeight(double width, double standard_tread_height=0) const;
		bool empty() const;
	private:
		double GetCadHeight(double width) const;
		//static 
		double cad_tread_height;//如果有后赋予的值，时刻注意要随时更新
		double cad_x_start;
		double cad_x_end;

		std::vector<Polygon> standard_flange_tread_msg;

	};
}