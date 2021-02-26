#pragma once
#include <vector>
#include <cmath>
#include <memory>
#include <string>
namespace Wheel {
	enum CircleDirection { Up, Down };

	class IHeightExtraction
	{
	public :
		virtual double GetHeight(double width) const = 0;
		virtual ~IHeightExtraction() = default;
	};

	class Point
	{
	public:
		Point(double x, double y) :x(x), y(y) {}
	//	double get_x() const { return x; }
	//	double get_y() const { return y; }
	//	double set_x(double x) { this->x = x; }
	//	double set_y(double y) { this->y = y; }
	//private:
		double x;
		double y;
	};

	class Circle : public IHeightExtraction
	{
	public:
		Circle(double a, double b, double r, double xmin, double xmax,CircleDirection direction)
			:a(a), b(b), r(r), xmin(xmin),xmax(xmax),direction(direction)
		{
			
		}
		
		double GetHeight(double width) const override;

	private:
		
		double a;
		double b;
		double r;
		double xmin;
		double xmax;
		CircleDirection direction;
	};

	class Line : public IHeightExtraction
	{
	public:
		Line(double x1, double y1, double x2, double y2) :x1(x1), y1(y1), x2(x2), y2(y2)
		{
		}
		double GetHeight(double width) const override;
	private:
		double get_xmin() const
		{
			return x1 > x2 ? x2 : x1;
		}
		double get_xmax() const
		{
			return x1 > x2 ? x1 : x2;
		}
		double x1;
		double y1;
		double x2;
		double y2;
	};

	//SH4L-SYÂÖÔµÌ¤Ãæ
	const double kRealTreadHeight = 393.6;
	const double kTreadHeight = 728.2251;
	const double kDy = kRealTreadHeight - kTreadHeight;
	const double kXStart = 2681.8007;
	const double kL = 135;

	class StandardFlangeTreadMsgMake
	{
	public:
		static const char kCadTreadHeightSymbol[];
		static const char kCadXStartSymbol[];
		static const char kCadXEndSymbol[];
		static const char kActualTreadHeightSymbol[];
		static const char kCircleSymbol[];
		static const char kLineSymbol[];

		StandardFlangeTreadMsgMake()
			:actual_tread_height(0),
			cad_tread_height(0),
			cad_x_start(0),
			cad_x_end(0),
			standard_tread_msg(),
			result(false),
			info_log("")
		{
		}
		explicit StandardFlangeTreadMsgMake(std::string file_path)
			:actual_tread_height(0), 
			cad_tread_height(0), 
			cad_x_start(0), 
			cad_x_end(0), 
			standard_tread_msg(),
			result(false),
			info_log("")
		{
			this->LoadMsg(file_path);
			//standard_tread_msg.push_back(std::make_shared<Circle>(2702.3007, 737.7164, 20.5000, 2681.8007, 2689.0360, CircleDirection::Up));//c1
			//standard_tread_msg.push_back(std::make_shared<Circle>(2696.8007, 744.1971, 12, 2689.0360, 2703.0857,CircleDirection::Up));//c2
			//standard_tread_msg.push_back(std::make_shared<Circle>(2692.6107, 737.3822, 20, 2703.0857, 2711.4046, CircleDirection::Up));//c3
			//standard_tread_msg.push_back(std::make_shared<Line>(2711.4046, 744.2226, 2712.8624, 740.2173));//l1
			//standard_tread_msg.push_back(std::make_shared<Circle>(2725.0784, 744.6635, 13, 2712.8624, 2717.4855, CircleDirection::Down));//c4
			//standard_tread_msg.push_back(std::make_shared<Circle>(2727.9988, 748.7221, 18, 2717.4855, 2724.6013, CircleDirection::Down));//c5
			//standard_tread_msg.push_back(std::make_shared<Circle>(2754.8007, 888.1697, 160, 2724.6013, 2751.3233, CircleDirection::Down));//c6
			//standard_tread_msg.push_back(std::make_shared<Line>(2751.3233, 728.2075, 2781.8007, 727.5450));//l2
			//standard_tread_msg.push_back(std::make_shared<Line>(2781.8007, 727.5450, 2811.8007, 725.5450));//l3
			//standard_tread_msg.push_back(std::make_shared<Line>(2811.8007, 725.5450, 2816.8007, 720.5457));//l4
		}
		double GetActualHeight(double width) const;
		double GetMakeZeroHeight(double width) const;
		bool get_result() const;
		std::string get_info_log() const;
	private:
		void LoadMsg(std::string file_path);
		std::vector<std::string> SplitString(const std::string& str, const std::string& separative) const;
		bool IsCount(char c) const;
		bool IsCount(const std::string& str) const;
		double ConvertToDouble(const std::string& str) const;
		//static 
		double actual_tread_height;
		double cad_tread_height;
		double cad_x_start;
		double cad_x_end;

		std::vector<std::shared_ptr<IHeightExtraction>> standard_tread_msg;

		//debug msg
		bool result;
		std::string info_log;
	};
}