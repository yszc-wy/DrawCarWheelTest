#pragma once
#include <glm/glm.hpp>

#include "DxfPolygon.h"
namespace DxfReader {
	class FloatPlantMsgMaker
	{
	public:
		static const double kFirstPointDisToTreadPoint;
		static const double kBothEndsReduceLength;

		FloatPlantMsgMaker():cad_tread_height(0), cad_x_start(0), float_plant_msg(){}
		FloatPlantMsgMaker(const std::vector<Polygon>& standard_tread_msg);
		std::vector<glm::vec2> GetFloatPlantPoints(double standard_tread_height) const;
		bool empty() const;
	private:
		//static 
		double cad_tread_height;
		double cad_x_start;

		std::vector<Polygon> float_plant_msg;
	};
}