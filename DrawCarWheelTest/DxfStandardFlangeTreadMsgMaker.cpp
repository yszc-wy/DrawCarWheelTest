#include "DxfStandardFlangeTreadMsgMaker.h"

#include <iostream>
namespace DxfReader {
	const double StandardFlangeTreadMsgMaker::kStandardTreadPointX = 70.0f;
	bool StandardFlangeTreadMsgMaker::empty() const
	{
		if (this->standard_flange_tread_msg.size() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	StandardFlangeTreadMsgMaker::StandardFlangeTreadMsgMaker(const std::vector<Polygon>& standard_tread_msg)
		:standard_flange_tread_msg(standard_tread_msg)
	{
		Point left_point;
		if (this->standard_flange_tread_msg.begin()->get_direction() == Direction::Forward)
		{
			left_point = this->standard_flange_tread_msg.begin()->get_p_ipolygon()->GetStartPoint();
		}
		else if (this->standard_flange_tread_msg.begin()->get_direction() == Direction::Reverse)
		{
			left_point = this->standard_flange_tread_msg.begin()->get_p_ipolygon()->GetEndPoint();
		}

		Point right_point;
		if (this->standard_flange_tread_msg.rbegin()->get_direction() == Direction::Forward)
		{
			right_point = this->standard_flange_tread_msg.rbegin()->get_p_ipolygon()->GetEndPoint();
		}
		else if (this->standard_flange_tread_msg.rbegin()->get_direction() == Direction::Reverse)
		{
			right_point = this->standard_flange_tread_msg.rbegin()->get_p_ipolygon()->GetStartPoint();
		}

		this->cad_x_start = left_point.x;
		this->cad_x_end = right_point.x;
		this->cad_tread_height = this->GetCadHeight(kStandardTreadPointX);


		//std::cout << "StandardFlangeTreadMsgMaker::OK?" << std::endl;
	}
	double StandardFlangeTreadMsgMaker::GetHeight(double width, double standard_tread_height) const
	{
		if (this->standard_flange_tread_msg.size()==0)
		{
			std::cout << "Error::StandardFlangeTreadMsgMaker::GetHeight::illegal msg make" << std::endl;
			return DBL_MAX;
		}

		double dy = 0 - cad_tread_height;//น้0

		//std::cout << "cad_x_start:"<< cad_x_start << std::endl;

		double real_width = width + this->cad_x_start;
		double height;
		for (auto i : standard_flange_tread_msg)
		{
			if ((height = i.get_p_ipolygon()->GetY(real_width)) != DBL_MAX)
			{
				break;
			}
		}
		if (height != DBL_MAX)
		{
			height += dy+ standard_tread_height;
		}
		else
		{
			std::cout << "Error::StandardFlangeTreadMsgMaker::GetHeight::width out of bound width:"<<width<< std::endl;
		}
		//std::cout << "width:" << width << " height:" << height << std::endl;
		return height;
	}

	double StandardFlangeTreadMsgMaker::GetCadHeight(double width) const
	{
		if (this->standard_flange_tread_msg.size() == 0)
		{
			std::cout << "Error::StandardFlangeTreadMsgMaker::GetHeight::illegal msg make" << std::endl;
			return DBL_MAX;
		}
		//std::cout << "StandardFlangeTreadMsgMaker::GetCadHeight::What?" << std::endl;
		//double dy = 0 - cad_tread_height;//น้0
		width += this->cad_x_start;

		//std::cout << "width:" << width << std::endl;

		double height;
		for (auto &i : standard_flange_tread_msg)
		{
			if ((height = i.get_p_ipolygon()->GetY(width)) != DBL_MAX)
			{
				//std::cout << "height:" << height << std::endl;
				break;
			}
			//std::cout << "height:" << height << std::endl;
		}
		//std::cout << "StandardFlangeTreadMsgMaker::GetCadHeight::Fuck?" << std::endl;
		if (height == DBL_MAX)
		{
			std::cout << "Error::StandardFlangeTreadMsgMaker::GetHeight::width out of bound" << std::endl;
		}

		//std::cout << "StandardFlangeTreadMsgMaker::GetCadHeight::End?" << std::endl;
		return height;
	}

}