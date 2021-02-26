#include "DxfFloatPlantMsgMaker.h"

#include <iostream>
#include <fstream>
namespace DxfReader {
	const double FloatPlantMsgMaker::kFirstPointDisToTreadPoint = 8.0f;
	const double FloatPlantMsgMaker::kBothEndsReduceLength = 30.0f;
	bool FloatPlantMsgMaker::empty() const
	{
		if (this->float_plant_msg.size() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	FloatPlantMsgMaker::FloatPlantMsgMaker(const std::vector<Polygon>& float_plant_msg)
		: float_plant_msg(float_plant_msg)
	{
		Point left_point;
		if (this->float_plant_msg.begin()->get_direction() == Direction::Forward)
		{
			left_point = this->float_plant_msg.begin()->get_p_ipolygon()->GetStartPoint();
			this->float_plant_msg.begin()->get_p_ipolygon()->DownStartPoint(kBothEndsReduceLength);//降低两边

		}
		else if (this->float_plant_msg.begin()->get_direction() == Direction::Reverse)
		{
			left_point = this->float_plant_msg.begin()->get_p_ipolygon()->GetEndPoint();
			this->float_plant_msg.begin()->get_p_ipolygon()->DownEndPoint(kBothEndsReduceLength);
		}

		this->cad_x_start = left_point.x;
		this->cad_tread_height = left_point.y - kFirstPointDisToTreadPoint;//得到cad踏面高度

		//for (auto &i : this->float_plant_msg)
		//{
		//	i.Show();
		//}
	}
	std::vector<glm::vec2> FloatPlantMsgMaker::GetFloatPlantPoints(double standard_tread_height) const
	{
		auto float_plant_msg_size = this->float_plant_msg.size();
		if (float_plant_msg_size == 0)
		{
			std::cout << "Error::FloatPlantMsgMaker::GetFloatPlantPoints::visit empty standard_flange_tread_msg" << std::endl;
			return std::vector<glm::vec2>();
		}
		std::vector<glm::vec2> points;
		//std::cout << "FloatPlantMsgMaker:" << std::endl;
		for (int i=0;i!= float_plant_msg_size;++i)
		{
			std::vector<glm::vec2> current_points;
			current_points=this->float_plant_msg[i].get_p_ipolygon()->GetUniformPoints(this->float_plant_msg[i].get_direction());
			points.insert(points.end(), current_points.begin(), current_points.end());

			//if (i == 0)
			//{
			//	this->float_plant_msg[0].Show();
			//	for (auto &i : current_points)
			//	{
			//		std::cout << "point: x:" << i.x << " y:" << i.y << std::endl;
			//	}
			//}
		}

		std::ofstream SaveFile("W://a.txt");
		SaveFile << "Point:" << std::endl;
		for (auto &i : points)
		{
			SaveFile << "x:" << i.x << " y:" << i.y << "\n";
		}
		SaveFile.close();

		for (auto &i : points)
		{
			i.x += -this->cad_x_start;
			i.y += -this->cad_tread_height + standard_tread_height;
		}

		std::ofstream openFile("W://b.txt");
		openFile << "Point:" << std::endl;
		for (auto &i : points)
		{
			openFile << "x:" << i.x << " y:" << i.y << "\n";
		}
		openFile.close();

		return points;
	}
}