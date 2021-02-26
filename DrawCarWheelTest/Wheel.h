#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <qlist.h>
#include <qvector2d.h>
#include <qvector.h>
#include <qmap.h>

#include "Vertex.h"
#include "Polar.h"
#include "Range.h"
#include "OpenglPointMsg.h"
#include "StandardFlangeTreadMsg.h"
#include "DxfStandardFlangeTreadMsgMaker.h"
//NJ840-2L
//NJ840-4L
//SH13LA
//SH840-4L
//SH840-135
namespace Wheel {
	const unsigned char kContrastColorModel = 1;
	const unsigned char kHeightColorModel = 0;

	class WheelColorContrast 
	{
	public:
		explicit WheelColorContrast():color_gamut(1.0f), flange_tread_r_range(),standard_tread(){}
		explicit WheelColorContrast(float color_gamut,const Range& spread_y_range, const std::vector<glm::vec2>& standard_tread) :
			color_gamut(color_gamut), flange_tread_r_range(spread_y_range),standard_tread(standard_tread)
		{
			if (color_gamut <= 0.0f)
			{
				std::cout << "Error::WheelColorContrast::WheelColorContrast::illegal color_gamut" << std::endl;
			}
		}
		explicit WheelColorContrast(const Range& spread_y_range, const std::vector<glm::vec2>& standard_tread) :
			color_gamut(1.0f), flange_tread_r_range(spread_y_range),standard_tread(standard_tread)
		{
		}
		explicit WheelColorContrast(const Range& spread_y_range) :
			color_gamut(1.0f), flange_tread_r_range(spread_y_range), standard_tread()
		{

		}
		void set_color_gamut(float color_gamut)
		{
			if (color_gamut <= 0.0f)
			{
				std::cout << "Error::WheelColorContrast::set_color_gamut::illegal color_gamut" << std::endl;
				return;
			}
			this->color_gamut = color_gamut;
		}
		float get_color_gamut() const
		{
			return this->color_gamut;
		}
		const std::vector<glm::vec2>& get_standard_tread() const
		{
			return standard_tread;
		}
		void set_standard_tread(const std::vector<glm::vec2>& standard_tread)
		{
			this->standard_tread = standard_tread;
		}
		void set_flange_tread_r_range(const Range& spread_y_range)
		{
			this->flange_tread_r_range;
		}
		Range get_flange_tread_r_range() const
		{
			return this->flange_tread_r_range;
		}
	private:
		float color_gamut;//色域（范围是 -color_gamut~color_gamut）

		//高度对比的y_range
		Range flange_tread_r_range;

		//标准踏面对比的踏面数据
		std::vector<glm::vec2> standard_tread;//被对比的标准踏面，如果标准轮数据为空，则选择高度图对比，不为空，选择标准轮对比
	};

	class WheelSpreadParm
	{
	public:
		explicit WheelSpreadParm() :unit_width(0), unit_length(0), length_range(), width_range(), color_contrast() {}
		explicit WheelSpreadParm(/*float wheelR,*/float unit_width, float unit_length, const Range& length_range,
			const Range& width_range, /*const Range& spread_y_range,*/const WheelColorContrast& color_contrast)
			:/*wheelR(wheelR),*/unit_width(unit_width), unit_length(unit_length), length_range(length_range), 
			width_range(width_range),/* spread_y_range(spread_y_range),*/ color_contrast(color_contrast) {}
		//float GetWheelR() const
		//{
		//	return wheelR;
		//}
		float get_unit_width() const
		{
			return unit_width;
		}
		float get_unit_length() const
		{
			return unit_length;
		}
		Range get_length_range() const
		{
			return length_range;
		}
		Range get_width_range() const
		{
			return width_range;
		}
		Range get_spread_y_range() const
		{
			return this->color_contrast.get_flange_tread_r_range();
		}
		const std::vector<glm::vec2>& get_standard_tread() const
		{
			return this->color_contrast.get_standard_tread();
		}
		void set_standard_tread(const std::vector<glm::vec2>& standard_tread)
		{
			this->color_contrast.set_standard_tread(standard_tread);
		}
		void set_color_gamut(float color_gamut)
		{
			this->color_contrast.set_color_gamut(color_gamut);
		}
		float get_color_gamut() const
		{
			return this->color_contrast.get_color_gamut();
		}
		
	private:

		//轮子展开参数
		float unit_width;
		float unit_length;
		Range length_range;
		Range width_range;
		

		//颜色对比参数
		//std::vector<glm::vec2> standardTreadMsg;
		WheelColorContrast color_contrast;
	};

	struct WheelSpreadMsg
	{
		OpenglObj::OpenglPointMsg wheelMsg;
		WheelSpreadParm wheelSpreadParm;
	};


	class WheelFlangeTread
	{
	public:
		static const float kPIR;
		static const float kFitBeginNode;//从32开始拟合，32之前的不变,一直拟合到surveyTreadBegin
		static const float kFitEndNode;//从surveyTreadEnd拟合到108.0f结束
		static const float kSurveyTreadBegin;
		static const float kSurveyTreadEnd;
		static const float kStandardTreadRadius;
		static const float kStandardFlangeTreadWidth;
		static const float kSpreadCircumferenceLength;
		static const int kFloatPlantSection;

	public:
		//Wheel(/*const std::string &path,const std::string &type*/);
		WheelFlangeTread(const QVector<QVector2D>& originSurveyFlange, const QMap<float, QVector<float>>& surveyTread/*, const QString type = "NJ840-2L"*/);
		~WheelFlangeTread();

		//加载轮缘踏面点
		OpenglObj::OpenglPointMsg LoadFlangeTreadPoint(/*std::map<float, std::vector<Polar>> &flangeTread, *//*std::vector<QVector2D> &floatPlant*/) const;//点的生成（包含与浮板的拼接）
		
		//加载展开的轮缘踏面点(数据已归0)																																	 //加载loadspread的时候不负责加载标准轮数据，应为其是可变的，可以在需要的时候用loadstandardtread动态添加
		WheelSpreadMsg LoadSpreadFlangeTreadPoint(/*const StandardFlangeTreadMsgMake& sft_msg_maker*/) const;

		//加载该轮子对应的标准轮信息(数据已归0)
		std::vector<glm::vec2> LoadStandardTread(const /*StandardFlangeTreadMsgMake*/DxfReader::StandardFlangeTreadMsgMaker& sft_msg_maker) const;

		//获取测量的踏面范围
		Range get_survey_tread_range() const;

		//获取轮缘踏面的高度范围
		Range get_flange_tread_r_range() const;//不归0

		//根据角度获取轮缘踏面的切片
		QVector<QVector2D> GetSection(float radian);

		const std::map<float, std::vector<Polar>>& get_flange_tread() const;

		
		//const std::vector<glm::vec2>& GetAverRadius() const;
		//const std::vector<Vertex>& GetVertices() const;
		//const std::vector<unsigned int>& GetIndices() const;
	private:
		void InitSurveyFlange(QVector<QVector2D>& surveyFlange);
		void InitFlangeTread(/*std::map<float, std::vector<Polar>> &flangeTread,*/const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, const std::vector<QVector2D> & standardOutsideTread);
		void InitStandardOutsideTread(std::vector<QVector2D> &standardOutsideTread);
		void InitFlangeTreadRRange();

		//保存加载出来的全部轮缘踏面信息，每一个宽度对应一圈的极坐标
		std::map<float, std::vector<Polar>> flange_tread;
		Range survey_tread_range;
		Range flange_tread_r_range; //不归0的数据

		//float GetAverRadius(float plie, float radian, std::map<float, std::vector<Polar>>& sampleData);
		//void InitStandardTreadData(std::map<float, std::vector<Polar>>&standardTread/*const std::string& type*/);
		//void InitSurveyTreadData(std::map<float, std::vector<Polar>>& surveyTread);
		//void InitFloatPlateData(/*std::vector<QVector2D> &floatPlant, */QString type);
		//void InitAverRadius(const QMap<float, QVector<float>>& surveyTread);
		//void InitStandardOtherData(std::vector<glm::vec2>& otherData);
		//std::vector<Vertex> vertices;
		//std::vector<unsigned int> indices;
		//std::vector<QVector2D> floatPlant;
		//std::vector<glm::vec2> averRadius;
		//void LoadPoint(std::map<float, std::vector<Polar>>& sampleData, std::map<float, std::vector<Polar>>& standardDrawData, std::vector<glm::vec2>& otherData);
		//void ConvertToPolar(float *)
		//float surveyTreadStart;
		//float surveyTreadEnd;
		//float minFlangeTreadY;
		//wheelSpread
	};

	//初始化浮板，以后可能要自动化加载
	std::vector<glm::vec2> InitFloatPlate(const std::string& filePath);

	//加载浮板点
	OpenglObj::OpenglPointMsg LoadFloatPlantPoint(const std::vector<glm::vec2>& floatPlant);

	//加载浮板和轮缘踏面之间的连接点
	OpenglObj::OpenglPointMsg LoadConnectionPoint(const std::map<float,std::vector<Polar>>& flangeTread,const std::vector<glm::vec2> & floatPlant);
}