#include "StandardFlangeTreadMsgMaker.h"

#include <iostream>
namespace DxfReader {
	const double StandardFlangeTreadMsgMaker::kStandardTreadPointX = 70.0f;
	StandardFlangeTreadMsgMaker::StandardFlangeTreadMsgMaker(const std::vector<Polygon>& standard_tread_msg)
		:cad_tread_height(cad_tread_height)
	{
		Point left_point;
		if (this->standard_flange_tread_msg.begin()->get_direction() == Direction::Forward)
		{
			left_point = this->standard_flange_tread_msg.begin()->get_start_point();
		}
		else if (this->standard_flange_tread_msg.begin()->get_direction() == Direction::Reverse)
		{
			left_point = this->standard_flange_tread_msg.begin()->get_end_point();
		}

		Point right_point;
		if (this->standard_flange_tread_msg.rbegin()->get_direction() == Direction::Forward)
		{
			right_point = this->standard_flange_tread_msg.rbegin()->get_end_point();
		}
		else if (this->standard_flange_tread_msg.rbegin()->get_direction() == Direction::Reverse)
		{
			right_point = this->standard_flange_tread_msg.rbegin()->get_start_point();
		}

		this->cad_x_start = left_point.x;
		this->cad_x_end = right_point.x;
		this->cad_tread_height = this->GetCadHeight(kStandardTreadPointX);
	}
	double StandardFlangeTreadMsgMaker::GetHeight(double width, double standard_tread_height = 0) const
	{
		if (this->standard_flange_tread_msg.size()==0)
		{
			std::cout << "Error::StandardFlangeTreadMsgMaker::GetHeight::illegal msg make" << std::endl;
			return DBL_MAX;
		}

		double dy = 0 - cad_tread_height;//น้0

		width += this->cad_x_start;
		double height;
		for (auto i : standard_flange_tread_msg)
		{
			if ((height = i.get_p_ipolygon()->GetY(width)) != FLT_MAX)
			{
				break;
			}
		}
		if (height != FLT_MAX)
		{
			height += dy+ standard_tread_height;
		}
		else
		{
			std::cout << "Error::StandardFlangeTreadMsgMaker::GetHeight::width out of bound" << std::endl;
		}
		return height;
	}

	double StandardFlangeTreadMsgMaker::GetCadHeight(double width) const
	{
		if (this->standard_flange_tread_msg.size() == 0)
		{
			std::cout << "Error::StandardFlangeTreadMsgMaker::GetHeight::illegal msg make" << std::endl;
			return DBL_MAX;
		}

		//double dy = 0 - cad_tread_height;//น้0

		width += this->cad_x_start;
		double height;
		for (auto i : standard_flange_tread_msg)
		{
			if ((height = i.get_p_ipolygon()->GetY(width)) != FLT_MAX)
			{
				break;
			}
		}
		if (height == FLT_MAX)
		{
			std::cout << "Error::StandardFlangeTreadMsgMaker::GetHeight::width out of bound" << std::endl;
		}
		return height;
	}

}