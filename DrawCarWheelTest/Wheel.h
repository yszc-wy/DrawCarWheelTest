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
		float color_gamut;//ɫ�򣨷�Χ�� -color_gamut~color_gamut��

		//�߶ȶԱȵ�y_range
		Range flange_tread_r_range;

		//��׼̤��Աȵ�̤������
		std::vector<glm::vec2> standard_tread;//���Աȵı�׼̤�棬�����׼������Ϊ�գ���ѡ��߶�ͼ�Աȣ���Ϊ�գ�ѡ���׼�ֶԱ�
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

		//����չ������
		float unit_width;
		float unit_length;
		Range length_range;
		Range width_range;
		

		//��ɫ�ԱȲ���
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
		static const float kFitBeginNode;//��32��ʼ��ϣ�32֮ǰ�Ĳ���,һֱ��ϵ�surveyTreadBegin
		static const float kFitEndNode;//��surveyTreadEnd��ϵ�108.0f����
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

		//������Ե̤���
		OpenglObj::OpenglPointMsg LoadFlangeTreadPoint(/*std::map<float, std::vector<Polar>> &flangeTread, *//*std::vector<QVector2D> &floatPlant*/) const;//������ɣ������븡���ƴ�ӣ�
		
		//����չ������Ե̤���(�����ѹ�0)																																	 //����loadspread��ʱ�򲻸�����ر�׼�����ݣ�ӦΪ���ǿɱ�ģ���������Ҫ��ʱ����loadstandardtread��̬���
		WheelSpreadMsg LoadSpreadFlangeTreadPoint(/*const StandardFlangeTreadMsgMake& sft_msg_maker*/) const;

		//���ظ����Ӷ�Ӧ�ı�׼����Ϣ(�����ѹ�0)
		std::vector<glm::vec2> LoadStandardTread(const /*StandardFlangeTreadMsgMake*/DxfReader::StandardFlangeTreadMsgMaker& sft_msg_maker) const;

		//��ȡ������̤�淶Χ
		Range get_survey_tread_range() const;

		//��ȡ��Ե̤��ĸ߶ȷ�Χ
		Range get_flange_tread_r_range() const;//����0

		//���ݽǶȻ�ȡ��Ե̤�����Ƭ
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

		//������س�����ȫ����Ե̤����Ϣ��ÿһ����ȶ�ӦһȦ�ļ�����
		std::map<float, std::vector<Polar>> flange_tread;
		Range survey_tread_range;
		Range flange_tread_r_range; //����0������

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

	//��ʼ�����壬�Ժ����Ҫ�Զ�������
	std::vector<glm::vec2> InitFloatPlate(const std::string& filePath);

	//���ظ����
	OpenglObj::OpenglPointMsg LoadFloatPlantPoint(const std::vector<glm::vec2>& floatPlant);

	//���ظ������Ե̤��֮������ӵ�
	OpenglObj::OpenglPointMsg LoadConnectionPoint(const std::map<float,std::vector<Polar>>& flangeTread,const std::vector<glm::vec2> & floatPlant);
}