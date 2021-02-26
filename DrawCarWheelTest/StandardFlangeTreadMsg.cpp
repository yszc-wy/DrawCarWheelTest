#include "StandardFlangeTreadMsg.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

#include <qstring.h>
namespace Wheel {

	const char StandardFlangeTreadMsgMake::kCadTreadHeightSymbol[]="#Cad_TreadHeight";
	const char StandardFlangeTreadMsgMake::kCadXStartSymbol[]="#Cad_XStart";
	const char StandardFlangeTreadMsgMake::kCadXEndSymbol[]="#Cad_XEnd";
	const char StandardFlangeTreadMsgMake::kActualTreadHeightSymbol[]="#Actual_TreadHeight";
	const char StandardFlangeTreadMsgMake::kCircleSymbol[]="#Circle";
	const char StandardFlangeTreadMsgMake::kLineSymbol[]="#Line";

	double Circle::GetHeight(double width) const
	{
		if (width<xmin || width>xmax)
		{
			return FLT_MAX;
		}
		double item = sqrt(pow(r, 2) - pow(width - a, 2));
		if (direction == Up)
		{
			return item + b;
		}
		else if (direction == Down)
		{
			return 0 - item + b;
		}
		else
		{
			return FLT_MAX;
		}
	}

	double Line::GetHeight(double width) const
	{
		if (width< get_xmin() || width> get_xmax())
		{
			return FLT_MAX;
		}

		double dx = x2 - x1;
		double dy = y2 - y1;
		return ((width - x1) / dx)*dy + y1;
	}

	bool StandardFlangeTreadMsgMake::get_result() const
	{
		return this->result;
	}

	std::string StandardFlangeTreadMsgMake::get_info_log() const
	{
		return this->info_log;
	}

	double StandardFlangeTreadMsgMake::GetActualHeight(double width) const
	{
		if (!this->result)
		{
			std::cout << "Error::StandardFlangeTreadMsgMake::GetActualHeight::illegal msg make" << std::endl;
			return DBL_MAX;
		}

		double dy = actual_tread_height - cad_tread_height;//归actual

		width += this->cad_x_start;
		double height;
		for (auto i : standard_tread_msg)
		{
			if ((height = i->GetHeight(width)) != FLT_MAX)
			{
				break;
			}
		}

		if (height != FLT_MAX)
		{
			height += dy;//归
		}
		else
		{
			std::cout << "Error::StandardFlangeTreadMsgMake::GetActualHeight::width out of bound" << std::endl;
		}
		return height;
	}
	double StandardFlangeTreadMsgMake::GetMakeZeroHeight(double width) const
	{
		if (!this->result)
		{
			std::cout << "Error::StandardFlangeTreadMsgMake::GetMakeZeroHeight::illegal msg make" << std::endl;
			return DBL_MAX;
		}

		double dy = 0- cad_tread_height;//归0

		width += this->cad_x_start;
		double height;
		for (auto i : standard_tread_msg)
		{
			if ((height = i->GetHeight(width)) != FLT_MAX)
			{
				break;
			}
		}
		if (height != FLT_MAX)
		{
			height += dy;
		}
		else
		{
			std::cout << "Error::StandardFlangeTreadMsgMake::GetActualHeight::width out of bound" << std::endl;
		}
		return height;
	}

	void StandardFlangeTreadMsgMake::LoadMsg(std::string file_path)
	{
		std::map<std::string, bool> judge;
		judge.insert({ kCadTreadHeightSymbol,false });
		judge.insert({ kCadXStartSymbol,false });
		judge.insert({ kCadXEndSymbol,false });
		judge.insert({ kActualTreadHeightSymbol,false });
		judge.insert({ "#CircleOrLine",false });

		std::ifstream file;
		file.open(file_path);
		//std::cout << "OK?" << std::endl;
		if (!file.is_open()) {
			this->info_log += "illegal file path!\n";
			std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::illegal file_path!" << std::endl;
			return;
			//std::cout << "Unable to open file" << std::endl;
		}
		std::string line;
		std::vector<std::string> splits;

		std::stringstream ss;

		while (getline(file, line)) // line中不包括每行的换行符  
		{
			if (line.size() == 0)
			{
				continue;
			}

			splits = this->SplitString(line, " ");
			//std::cout << "OK1" << std::endl;
			if (splits[0] == kCadTreadHeightSymbol)
			{
				
				if (splits.size() - 1 != 1)
				{
					this->info_log += "#CadTreadHeight::illegal varible count\n";
					std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kCadTreadHeightSymbol::illegal varible count" << std::endl;
					return;
				}
				else
				{
					this->cad_tread_height = ConvertToDouble(splits[1]);
					if (this->cad_tread_height == DBL_MAX)
					{
						this->info_log += "#CadTreadHeight::illegal varible::can not convert to double\n";
						return;
					}
					judge[kCadTreadHeightSymbol] = true;
				}
			}
			else if (splits[0] == kCadXStartSymbol)
			{
				
				if (splits.size() - 1 != 1)
				{
					this->info_log += "#CadXStart::illegal varible count\n";
					std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kCadXStartSymbol::illegal varible count" << std::endl;
					return;
				}
				else
				{
					this->cad_x_start = ConvertToDouble(splits[1]);
					if (this->cad_x_start == DBL_MAX)
					{
						this->info_log += "#CadXStart::illegal varible::can not convert to double\n";
						return;
					}
					judge[kCadXStartSymbol] = true;
				}
			}
			else if (splits[0] == kCadXEndSymbol)
			{
				
				if (splits.size() - 1 != 1)
				{
					this->info_log += "#CadXEnd::illegal varible count\n";
					std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kCadXEndSymbol::illegal varible count" << std::endl;
					return;
				}
				else
				{
					
					this->cad_x_end = ConvertToDouble(splits[1]);
					if (this->cad_x_end == DBL_MAX)
					{
						this->info_log += "#CadXEnd::illegal varible::can not convert to double\n";
						return;
					}
					judge[kCadXEndSymbol] = true;
				}
			}
			else if (splits[0] == kActualTreadHeightSymbol)
			{
				
				if (splits.size() - 1 != 1)
				{
					this->info_log += "#ActualTreadHeight::illegal varible count\n";
					std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kActualTreadHeightSymbol::illegal varible count" << std::endl;
					return;
				}
				else
				{
					this->actual_tread_height = ConvertToDouble(splits[1]);
					if (this->actual_tread_height == DBL_MAX)
					{
						this->info_log += "#ActualTreadHeight::illegal varible::can not convert to double\n";
						return;
					}
					judge[kActualTreadHeightSymbol] = true;
				}
			}
			else if (splits[0] == kCircleSymbol)
			{

				if (splits.size() - 1 != 6)
				{
					this->info_log += "#Circle::illegal varible count\n";
					std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kCircleSymbol::illegal varible count" << std::endl;
					return;
				}
				else
				{

					double a = ConvertToDouble(splits[1]);
					double b = ConvertToDouble(splits[2]);
					double r = ConvertToDouble(splits[3]);
					double xmin= ConvertToDouble(splits[4]);
					double xmax = ConvertToDouble(splits[5]);
					if (a == DBL_MAX || b == DBL_MAX || r == DBL_MAX || xmin == DBL_MAX || xmax == DBL_MAX)
					{
						this->info_log += "#Circle::illegal varible::can not convert to double\n";
						//std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kLineSymbol::illegal varible count" << std::endl;
						return;
					}

					CircleDirection direction;
					if (splits[6] == "Up")
					{
						direction = CircleDirection::Up;
					}
					else if(splits[6] == "Down")
					{
						direction = CircleDirection::Down;
					}
					else
					{
						this->info_log += "#Circle::illegal varible::circle direction error\n";
						std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kCircleSymbol::illegal varible" << std::endl;
						return;
					}
					judge["#CircleOrLine"] = true;

					this->standard_tread_msg.push_back(std::make_shared<Circle>(a, b, r, xmin, xmax, direction));
				}
			}
			else if (splits[0] == kLineSymbol)
			{
				if (splits.size() - 1 != 4)
				{
					this->info_log += "#Line::illegal varible count\n";
					std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kLineSymbol::illegal varible count" << std::endl;
					return;
				}
				else
				{
					double x1 = ConvertToDouble(splits[1]);
					double y1 = ConvertToDouble(splits[2]);
					double x2 = ConvertToDouble(splits[3]);
					double y2 = ConvertToDouble(splits[4]);
					if (x1 == DBL_MAX || y1 == DBL_MAX || x2 == DBL_MAX || y2 == DBL_MAX)
					{
						this->info_log += "#Line::illegal varible::can not convert to double\n";
						//std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg::kLineSymbol::illegal varible count" << std::endl;
						return;
					}

					judge["#CircleOrLine"] = true;

					this->standard_tread_msg.push_back(std::make_shared<Line>(x1, y1, x2, y2));
				}
			}
			else
			{
				//std::cout << "OK2" << std::endl;
				continue;
			}
			//std::cout << "OK2" << std::endl;
		}

		for (auto &j : judge)
		{ 
			if (!j.second)
			{
				this->info_log += "Error::" + j.first + " unfind\n";
				std::cout << "Error::StandardFlangeTreadMsgMake::LoadMsg:: " << j.first << " unfind" << std::endl;
				return;
			}
		}

		this->result = true;
		//std::cout << "OK!" << std::endl;
	}

	std::vector<std::string> StandardFlangeTreadMsgMake::SplitString(const std::string& str, const std::string& separative) const
	{
		std::string::size_type pos1, pos2;
		pos2 = str.find(separative);
		pos1 = 0;

		std::vector<std::string> splits;
		while (std::string::npos != pos2)
		{
			splits.push_back(str.substr(pos1, pos2 - pos1));

			pos1 = pos2 + separative.size();
			pos2 = str.find(separative, pos1);
		}
		if (pos1 != str.size())
			splits.push_back(str.substr(pos1));

		return splits;
	}

	bool StandardFlangeTreadMsgMake::IsCount(char c) const
	{
		if (c >= '0'&&c <= '9')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool StandardFlangeTreadMsgMake::IsCount(const std::string& str) const
	{
		auto str_size = str.size();
		if (str_size == 0)
		{
			return false;
		}
		if (str_size == 1)
		{
			if (IsCount(str[0]))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		if (!(IsCount(str[0]) && IsCount(str[str_size-1])))  //判断两边
		{
			return false;
		}
		int point_count = 0;
		for (int i = 1; i != str_size - 1; ++i)
		{
			if (str[i] == '.')
			{
				++point_count;
				continue;
			}
			else if(IsCount(str[i]))
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		if (point_count > 1)
		{
			return false;
		}
		return true;
	}

	double StandardFlangeTreadMsgMake::ConvertToDouble(const std::string& str) const
	{
		if (!IsCount(str))
		{

			std::cout << "Error::StandardFlangeTreadMsgMake::ConvertToDouble::can not convert to double" << std::endl;
			return DBL_MAX;
		}
		double num = DBL_MAX;
		std::stringstream ss(str);
		ss >> num;
		return num;
	}

}