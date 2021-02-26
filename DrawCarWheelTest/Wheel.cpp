#include "Wheel.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <memory>

#include "qstring.h"
#include "qmessagebox.h"
#include "qiterator.h"

#include "FilePath.h"
#include "StandardFlangeTreadMsg.h"
namespace Wheel {
	//const unsigned int COLUMN=496;
	//const unsigned int PLIECOUNT=44;
	//const unsigned int ROW = PLIECOUNT *2;
	//
	//const float STARTSAMPLEPLIE = 29;
	//const float ENDSAMPLEPLIE = 115;
	//const float SAMPLESPACING = 1;//mm
	//
	//const float PIR = 3.1415926;
	//const unsigned int CIRCLECOUNT = 629;
	//float sampleSourceMsg[COLUMN][ROW];
	//const unsigned int STANDARDPILECOUNT = 131;
	//float standardWheelMsg[STANDARDPILECOUNT];

	const float WheelFlangeTread::kPIR = 3.1415926;
	const float WheelFlangeTread::kFitBeginNode = 31.0f;//从32开始拟合，32之前的不变,一直拟合到surveyTreadBegin
	const float WheelFlangeTread::kFitEndNode = 108.0f;//从surveyTreadEnd拟合到108.0f结束
	const float WheelFlangeTread::kSurveyTreadBegin = 50;
	const float WheelFlangeTread::kSurveyTreadEnd = 80;
	const float WheelFlangeTread::kStandardTreadRadius = 393.6;  //后面要改！
	const float WheelFlangeTread::kStandardFlangeTreadWidth = 135;
	const float WheelFlangeTread::kSpreadCircumferenceLength = 600;  //展开的圆周宽度
	const int WheelFlangeTread::kFloatPlantSection = 720;

	//Wheel::Wheel(/*const std::string &sampleDataPath,  const std::string &type*/)
	//{
	//	std::ifstream file("./Resources/CadMsg/SurveyTreadData.txt");
	//	if (!file.is_open()) {
	//		QString error = "ERROR::WHEEL::SOURCEMSG::UNABLE TO OPEN FILE";
	//		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	//		//std::cout << "Unable to open sourceMsg file"<<std::endl;  
	//	}
	//	float count;
	//	float *p = &sampleSourceMsg[0][0];
	//	while (file>>count)
	//	{
	//		*p = count;
	//		++p;
	//	}
	//	file.close();
	//
	//	file.open("./Resources/CadMsg/StandardTreadData.txt");
	//	if (!file.is_open()) {
	//		QString error = "ERROR::WHEEL::STANDARDMSG::UNABLE TO OPEN FILE";
	//		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	//		//std::cout << "Unable to open file" << std::endl;
	//	}
	//	float pile;
	//	float r;
	//	p = &standardWheelMsg[0];
	//	while (file >> pile >> r)
	//	{
	//		*p = r;
	//		++p;
	//	}
	//	file.close();
	//
	//	std::vector<glm::vec2> otherData;
	//
	//	//NJ840-2L
	//	//NJ840-4L
	//	//SH13LA
	//	//SH840-4L
	//	//SH840-135
	//	file.open("./Resources/CadMsg/SH840-135.txt");
	//	if (!file.is_open()) {
	//		QString error = "ERROR::WHEEL::FLOATPLATE::UNABLE TO OPEN FILE";
	//		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	//		//std::cout << "Unable to open file" << std::endl;
	//	}
	//	float x, y;
	//	while (file >> x >> y)
	//	{
	//		otherData.push_back(glm::vec2(x, y));
	//	}
	//	file.close();
	//
	//	std::map<float, std::vector<Polar>> sampleData;//规范化(除0排序)后的采样数据
	//
	//	InitSurveyTreadData(sampleData);
	//
	//	std::map<float, std::vector<Polar>> standardDrawData;//标准0.01角度的绘图数据,用标准轮数据初始化
	//
	//	InitStandardTreadData(standardDrawData);
	//
	//	LoadPoint(sampleData, standardDrawData,otherData);
	//}
	Range WheelFlangeTread::get_survey_tread_range() const
	{
		return this->survey_tread_range;
	}
	//const std::vector<glm::vec2>& Wheel::GetAverRadius() const
	//{
	//	return averRadius;
	//}
	WheelFlangeTread::WheelFlangeTread(const QVector<QVector2D>& originSurveyFlange, const QMap<float, QVector<float>>& surveyTread/*, const QString type*/)
	{
		this->survey_tread_range.SetRange(surveyTread.firstKey(), surveyTread.lastKey());

		//std::cout << "TreadStart:" << this->survey_tread_range.GetStart() << " SurveyTreadEnd:" << this->survey_tread_range.GetEnd() << std::endl;
		//this->InitAverRadius(surveyTread);

		QVector<QVector2D> surveyFlange = originSurveyFlange;
		this->InitSurveyFlange(surveyFlange);

		std::vector<QVector2D> standardOutsideTread;
		this->InitStandardOutsideTread(standardOutsideTread);

		//std::map<float, std::vector<Polar>> flangeTread;
		this->InitFlangeTread(/*flangeTread, */surveyFlange, surveyTread, standardOutsideTread);//拼接拟合flange和tread,再与OutsideTread拟合

		this->InitFlangeTreadRRange();
		//std::vector<QVector2D> floatPlant;
		//this->InitFloatPlateData(/*floatPlant, */type);

		//this->LoadPoint(/*flangeTread, *//*floatPlant*/);
	}
	//void Wheel::InitAverRadius(const QMap<float, QVector<float>>& surveyTread)
	//{
	//	int section = surveyTread.begin().value().size();
	//	float unitRadian = 2 * PIR / (float)section;

	//	QMap<float, QVector<float>>::const_iterator map_iterator;
	//	auto begin = surveyTread.constBegin();
	//	auto end = surveyTread.constEnd();
	//	for (map_iterator = begin; map_iterator != end; ++map_iterator)
	//	{
	//		glm::vec2 averPoint;
	//		averPoint.x = map_iterator.key();
	//		double sum = 0;

	//		for (int j = 0; j != section; ++j)//多少截面
	//		{
	//			sum += map_iterator.value()[j];
	//		}
	//		averPoint.y = sum / section;
	//		this->averRadius.push_back(averPoint);
	//		//std::cout << averPoint.y << std::endl;
	//		//std::cout << "AverRadius.x:" << averPoint.x << "AverRadius.y:" << averPoint.y << std::endl;
	//	}

	//}
	void WheelFlangeTread::InitSurveyFlange(QVector<QVector2D>& surveyFlange)
	{
		float dx = 0 - surveyFlange[0].x();
		int id = -1;
		int surveyFlangeSize = surveyFlange.size();
		for (int i = 0; i != surveyFlangeSize; ++i)
		{
			surveyFlange[i].setX(surveyFlange[i].x() + dx);//平移X校准
			if (surveyFlange[i].x() >= survey_tread_range.GetStart())//截断超范围数据
			{
				id = i;
				break;
			}
		}
		surveyFlange.erase(surveyFlange.begin() + id, surveyFlange.end());
	}
	void WheelFlangeTread::InitStandardOutsideTread(std::vector<QVector2D> &standardOutsideTread)//读取外踏面数据
	{
		std::ifstream file;
		file.open(FilePath::kStandardOutsideTreadPath);
		if (!file.is_open()) {
			QString error = "ERROR::WHEEL::STANDARDOUTSIDETREAD::UNABLE TO OPEN FILE";
			QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
			//std::cout << "Unable to open file" << std::endl;
		}
		float x, y;
		float realTread;
		file >> realTread;
		float dy = kStandardTreadRadius - realTread;//半径校准
		while (file >> x >> y)
		{
			if (x <= this->survey_tread_range.GetEnd())//截断超范围数据
			{
				continue;
			}
			standardOutsideTread.push_back(QVector2D(x, y + dy));//调整参照半径
		}
		file.close();
	}
	const std::map<float, std::vector<Polar>>& WheelFlangeTread::get_flange_tread() const
	{
		return this->flange_tread;
	}
	//void Wheel::InitFloatPlateData(/*std::vector<QVector2D> &floatPlant,*/QString type)
	//{
	//	std::ifstream file;
	//	QString directory = FilePath::kFloatPlateDirectory + type + ".txt";
	//	file.open(directory.toStdString());
	//	if (!file.is_open()) {
	//		QString error = "ERROR::WHEEL::FLOATPLATE::UNABLE TO OPEN FILE,MAY BE THIS FLOAT PLANT TYPE IS UNFINED";
	//		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	//		//std::cout << "Unable to open file" << std::endl;
	//	}
	//	float x, y;
	//	float realTread;
	//	file >> realTread;
	//	float dy = STANDARDTREADRADIUS - realTread;
	//	while (file >> x >> y)
	//	{
	//		this->floatPlant.push_back(QVector2D(x, y + dy));
	//	}
	//	file.close();
	//}
	void WheelFlangeTread::InitFlangeTread(/*std::map<float, std::vector<Polar>> &flangeTread,*/const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float> >& surveyTread, const std::vector<QVector2D> & standardOutsideTread)
	{

		int section = surveyTread.begin().value().size();
		float unitRadian = 2 * kPIR / (float)section;

		int fitFlangePointSize = 0;//需要拟合操作的点数量
		int fitFlangePointBeginI = 0;
		bool first = true;

		int surveyFlangeCircleSize = surveyFlange.size();

		for (int i = 0; i != surveyFlangeCircleSize; ++i)//将激光数据导入轮缘踏面数据集_SurveyFlangeTread
		{
			float radian = 0;
			std::vector<Polar> circlePoint;
			for (int j = 0; j != section; ++j)//多少截面
			{
				circlePoint.push_back(Polar(surveyFlange[i].y(), radian));
				radian += unitRadian;
			}
			if (surveyFlange[i].x() > kFitBeginNode)
			{
				if (first)
				{
					first = false;
					fitFlangePointBeginI = i;
				}
				++fitFlangePointSize;
			}
			this->flange_tread.insert({ surveyFlange[i].x(),circlePoint }); //surveyFlange[i].x<=50mm
		}

		//将SurveyTread导入FlangeTread,x范围surveyTreadBegin-surveyTreadEnd

		QMap<float, QVector<float>>::const_iterator map_iterator;
		auto begin = surveyTread.constBegin();
		auto end = surveyTread.constEnd();
		for (map_iterator = begin; map_iterator != end; ++map_iterator) {
			const QVector<float>& r = map_iterator.value();
			float radian = 0;
			std::vector<Polar> circlePoint;
			for (int j = 0; j != section; ++j)//多少截面
			{
				circlePoint.push_back(Polar(r[j], radian));
				radian += unitRadian;
			}
			this->flange_tread.insert({ map_iterator.key(),circlePoint });
		}

		//QList<float>::iterator p = surveyTread.begin();
		//float radian = 0;
		//std::vector<Polar> circlePoint;
		//for (int i = 0; i != section; ++i)//多少截面
		//{
		//	circlePoint.push_back(Polar(*p, radian));
		//	++p;
		//	radian += unitRadian;
		//}
		//flangeTread.insert({ 70.0f ,circlePoint });
		float surveyTreadMinX = surveyTread.firstKey();
		float surveyTreadMaxX = surveyTread.lastKey();

		//拟合左侧激光到踏面数据
		//获取激光的最大x值的y值
		float nearY = surveyFlange[surveyFlangeCircleSize - 1].y();
		std::vector<Polar>& leftTread = this->flange_tread[surveyTreadMinX];//SurveyTread最左边的一圈点
		for (int i = 0; i != section; ++i)//拟合
		{
			float dr = leftTread[i].r - nearY;
			float ddr = dr / fitFlangePointSize;
			float k = 0;
			for (int j = fitFlangePointBeginI; j != surveyFlangeCircleSize; ++j)//拟合到SurveyFlange末尾
			{
				this->flange_tread[surveyFlange[j].x()][i].r = surveyFlange[j].y() + ddr*(k++);
			}
		}

		//导入标准外踏面数据，再用tread与其拟合
		int standardOutsideTreadCircleSize = standardOutsideTread.size();
		int fitOutsideTreadPointSize = 0;//需要拟合操作的点数量
		int fitOutsideTreadPointEndI = 0;
		first = true;

		for (int i = 0; i != standardOutsideTreadCircleSize; ++i)
		{
			float radian = 0;
			std::vector<Polar> circlePoint;
			for (int j = 0; j != section; ++j)//多少截面
			{
				circlePoint.push_back(Polar(standardOutsideTread[i].y(), radian));
				radian += unitRadian;
			}
			if (standardOutsideTread[i].x() < kFitEndNode)
			{
				++fitOutsideTreadPointSize;
			}
			else
			{
				if (first)
				{
					first = false;
					fitOutsideTreadPointEndI = i;
				}
			}
			this->flange_tread.insert({ standardOutsideTread[i].x(),circlePoint }); //surveyFlange[i].x<=50mm
		}

		//拟合右边
		nearY = standardOutsideTread[0].y();
		std::vector<Polar>& rightTread = this->flange_tread[surveyTreadMaxX];//SurveyTread最右边的一圈点
		for (int i = 0; i != section; ++i)//拟合
		{
			float dr = rightTread[i].r - nearY;
			float ddr = dr / fitOutsideTreadPointSize;
			float k = 0;
			for (int j = fitOutsideTreadPointEndI - 1; j >= 0; --j)//拟合到SurveyFlange末尾
			{
				this->flange_tread[standardOutsideTread[j].x()][i].r = standardOutsideTread[j].y() + ddr*(k++);
			}
		}
	}
	std::vector<glm::vec2> WheelFlangeTread::LoadStandardTread(const DxfReader::StandardFlangeTreadMsgMaker& sft_msg_maker) const
	{
		std::vector<glm::vec2> standardTreadMsg;
		//StandardFlangeTreadMsgMake stmMake(FilePath::kExampleStandardTreadPath);

		auto begin = this->flange_tread.cbegin();
		auto end = this->flange_tread.cend();
		for (std::map<float, std::vector<Polar>>::const_iterator i = begin; i != end; ++i)//每一圈
		{
			standardTreadMsg.push_back(glm::vec2(i->first, sft_msg_maker.GetHeight(i->first)));
		}
		return standardTreadMsg;
	}
	WheelSpreadMsg WheelFlangeTread::LoadSpreadFlangeTreadPoint(/*const StandardFlangeTreadMsgMake& sft_msg_maker*/) const  
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		//std::vector<glm::vec2> standardTreadMsg;

		int flangeTreadCircleSize = this->flange_tread.size();
		int section = this->flange_tread.begin()->second.size();
		//float circumference = 2.0f * PIR* STANDARDTREADRADIUS;
		float circumference = kSpreadCircumferenceLength;
		float unitDis = circumference / static_cast<float>(section);
		float startX = 0;
		float endX = circumference;
		//
		//StandardFlangeTreadMsgMake stmMake(FilePath::kExampleStandardTreadPath);

		//flangeTread转顶点,	//寻找最大和最小Y
		//bool first = true;
		//float minY;
		//float maxY;

		auto begin = this->flange_tread.cbegin();
		auto end = this->flange_tread.cend();
		std::vector<Vertex> ciclePoint;
		Vertex vertex;
		for (std::map<float, std::vector<Polar>>::const_iterator i = begin; i != end; ++i)//每一圈
		{
			vertex.Normal = glm::vec3(0.0f);
			for (int j = 0; j != section; ++j)  //每一圈上的点
			{
				//if (first)
				//{
				//	maxY = i->second[j].r - STANDARDTREADRADIUS;//高度数据除去
				//	minY = i->second[j].r - STANDARDTREADRADIUS;
				//	first = false;
				//}
				//else
				//{
				//	if (i->second[j].r > maxY)
				//	{
				//		maxY = i->second[j].r - STANDARDTREADRADIUS;
				//	}
				//	if (i->second[j].r < minY)
				//	{
				//		minY = i->second[j].r - STANDARDTREADRADIUS;
				//	}
				//}

				vertex.Position = glm::vec3(startX + j*unitDis, i->second[j].r - kStandardTreadRadius, i->first);//删去了
				ciclePoint.push_back(vertex);
			}
			//standardTreadMsg.push_back(glm::vec2(i->first, sft_msg_maker.GetMakeZeroHeight(i->first)));
			vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
			ciclePoint.clear();
		}
		//std::cout << "MsgCount:" << standardTreadMsg.size() << std::endl;
		//test
		//std::cout << "Msg:" << std::endl;
		//for (auto i : standardTreadMsg)
		//{
		//	std::cout << i.x << " " << i.y << std::endl;
		//}


		size_t flangeTreadVerticesSize = vertices.size();//得到flangeTread总共的点数
		//flangeTread的indices
		for (int i = 0; i != flangeTreadCircleSize - 1; ++i)//最后一层不用操作
		{
			int currentMuit = section * i;
			int nextMuit = section * (i + 1);
			for (int j = 0; j != section - 1; ++j)  //不连接
			{
				//正方形第一个三角形
				indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
				indices.push_back(nextMuit + (j + 1) % section); //nextMuit + (j + 1) % section
				indices.push_back(nextMuit + j);//nextMuit + j
												//正方形第二个三角形
				indices.push_back(currentMuit + j);
				indices.push_back(currentMuit + (j + 1) % section);//currentMuit + (j + 1) % section
				indices.push_back(nextMuit + (j + 1) % section);//nextMuit + (j + 1) % section
			}
		}
		size_t flangeTreadIndicesSize = indices.size();

		//计算flangeTread法向量
		glm::vec3 a, b;
		for (int i = 0; i < flangeTreadIndicesSize; i += 3)
		{
			a = vertices[indices[i + 1]].Position - vertices[indices[i]].Position;
			b = vertices[indices[i + 2]].Position - vertices[indices[i + 1]].Position;

			vertices[indices[i]].Normal += glm::cross(a, b);
			vertices[indices[i + 1]].Normal += glm::cross(a, b);
			vertices[indices[i + 2]].Normal += glm::cross(a, b);
		}

		WheelSpreadMsg smg;
		smg.wheelMsg.SetMsg_Move(std::move(vertices), std::move(indices));
		smg.wheelSpreadParm = WheelSpreadParm(0.05, unitDis, Range(startX, endX), this->survey_tread_range, 
			WheelColorContrast(Range(this->flange_tread_r_range.GetMin() - kStandardTreadRadius, this->flange_tread_r_range.GetMax() - kStandardTreadRadius)));
		return smg;
	}

	void WheelFlangeTread::InitFlangeTreadRRange()
	{
		bool first = true;
		float minR;
		float maxR;
		auto begin = this->flange_tread.cbegin();
		auto end = this->flange_tread.cend();
		int section = this->flange_tread.begin()->second.size();
		for (std::map<float, std::vector<Polar>>::const_iterator i = begin; i != end; ++i)//每一圈
		{
			for (int j = 0; j != section; ++j)  //每一圈上的点
			{
				if (first)
				{
					maxR = i->second[j].r;
					minR = i->second[j].r;
					first = false;
				}
				else
				{
					if (i->second[j].r > maxR)
					{
						maxR = i->second[j].r;
					}
					if (i->second[j].r < minR)
					{
						minR = i->second[j].r;
					}
				}
			}
		}
		this->flange_tread_r_range = Range(minR, maxR);
	}

	Range WheelFlangeTread::get_flange_tread_r_range() const
	{
		return this->flange_tread_r_range;
	}
	//WheelOpenglMsg Wheel::LoadConnection()
	//{
	//	std::vector<Vertex> vertices;
	//	std::vector<unsigned int> indices;

	//	int section = this->flangeTread.begin()->second.size();
	//	float unitRadian = 2 * PIR / (float)section;
	//	std::vector<Vertex> ciclePoint;
	//	Vertex vertex;

	//	//轮缘踏面第一圈的连接点
	//	std::map<float, std::vector<Polar>>::iterator firstCircle = this->flangeTread.begin();
	//	vertex.Normal = glm::vec3(0.0f);
	//	for (int i = 0; i != section; ++i)  //每一圈上的点
	//	{
	//		vertex.Position = glm::vec3(firstCircle->second[i].ConvertToXY(), firstCircle->first);
	//		ciclePoint.push_back(vertex);
	//	}
	//	vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
	//	ciclePoint.clear();


	//}
	//WheelOpenglMsg Wheel::LoadFloatPlantPoint()
	//{
	//	std::vector<Vertex> vertices;
	//	std::vector<unsigned int> indices;

	//	int section = this->flangeTread.begin()->second.size();
	//	float unitRadian = 2 * PIR / (float)section;
	//	std::vector<Vertex> ciclePoint;
	//	Vertex vertex;

	//	//第一圈的连接点
	//	std::map<float, std::vector<Polar>>::iterator firstCircle= this->flangeTread.begin();
	//	vertex.Normal = glm::vec3(0.0f);
	//	for (int i = 0; i != section; ++i)  //每一圈上的点
	//	{
	//		vertex.Position = glm::vec3(firstCircle->second[i].ConvertToXY(), firstCircle->first);
	//		ciclePoint.push_back(vertex);
	//	}
	//	vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
	//	ciclePoint.clear();
	//	////
	//	//floatPlant转顶点
	//	size_t floatPlantCircleSize = this->floatPlant.size();
	//	Polar polar;
	//	for (int i = 0; i != floatPlantCircleSize; ++i)
	//	{
	//		//std::vector<Vertex> ciclePoint;
	//		polar.r = this->floatPlant[i].y();

	//		float radian = 0;
	//		for (int j = 0; j != section; ++j)  //每一圈上的点
	//		{
	//			polar.radian = radian;
	//			vertex.Position = glm::vec3(polar.ConvertToXY(), this->floatPlant[i].x());
	//			ciclePoint.push_back(vertex);
	//			radian += unitRadian;
	//		}
	//		vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
	//		ciclePoint.clear();
	//	}
	//	//第二圈的连接点
	//	std::map<float, std::vector<Polar>>::reverse_iterator lastCircle = this->flangeTread.rbegin();
	//	vertex.Normal = glm::vec3(0.0f);
	//	for (int i = 0; i != section; ++i)  //每一圈上的点
	//	{
	//		vertex.Position = glm::vec3(lastCircle->second[i].ConvertToXY(), lastCircle->first);
	//		ciclePoint.push_back(vertex);
	//	}
	//	vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
	//	ciclePoint.clear();
	//	////


	//	//floatPlant的indices
	//	for (int i = 0; i != floatPlantCircleSize + 2 - 1; ++i)//最后一层不要操作
	//	{
	//		int currentMuit = section*i;
	//		int nextMuit = section * (i + 1);

	//		for (int j = 0; j != section; ++j)
	//		{
	//			//正方形第一个三角形
	//			indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
	//			indices.push_back(nextMuit + j);
	//			indices.push_back(nextMuit + (j + 1) % section);
	//			//正方形第二个三角形
	//			indices.push_back(currentMuit + j);
	//			indices.push_back(nextMuit + (j + 1) % section);
	//			indices.push_back(currentMuit + (j + 1) % section);
	//		}

	//	}

	//	//计算floatPlant法向量
	//	glm::vec3 a, b;
	//	for (int i = 0; i < indices.size(); i += 3)
	//	{
	//		a = vertices[indices[i + 1]].Position - vertices[indices[i]].Position;
	//		b = vertices[indices[i + 2]].Position - vertices[indices[i + 1]].Position;

	//		vertices[indices[i]].Normal += glm::cross(a, b);
	//		vertices[indices[i + 1]].Normal += glm::cross(a, b);
	//		vertices[indices[i + 2]].Normal += glm::cross(a, b);
	//	}

	//	return WheelOpenglMsg(std::move(vertices), std::move(indices));
	//}
	OpenglObj::OpenglPointMsg WheelFlangeTread::LoadFlangeTreadPoint(/*std::map<float, std::vector<Polar>> &flangeTread,*/ /*std::vector<QVector2D> &floatPlant*/) const
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		int flangeTreadCircleSize = this->flange_tread.size();
		int section = this->flange_tread.begin()->second.size();
		float unitRadian = 2 * kPIR / (float)section;

		//flangeTread转顶点
		auto begin = this->flange_tread.cbegin();
		auto end = this->flange_tread.cend();
		std::vector<Vertex> ciclePoint;
		Vertex vertex;
		for (std::map<float, std::vector<Polar>>::const_iterator i = begin; i != end; ++i)//每一圈
		{
			vertex.Normal = glm::vec3(0.0f);
			for (int j = 0; j != section; ++j)  //每一圈上的点
			{
				vertex.Position = glm::vec3(i->second[j].ConvertToXY(), i->first);
				ciclePoint.push_back(vertex);
			}
			vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
			ciclePoint.clear();
		}
		size_t flangeTreadVerticesSize = vertices.size();//得到flangeTread总共的点数

		////floatPlant转顶点
		//size_t floatPlantCircleSize = this->floatPlant.size();
		//Polar polar;
		//for (int i = 0; i != floatPlantCircleSize; ++i)
		//{
		//	//std::vector<Vertex> ciclePoint;
		//	polar.r = this->floatPlant[i].y();
		//	Vertex vertex;

		//	float radian = 0;
		//	for (int j = 0; j != section; ++j)  //每一圈上的点
		//	{
		//		polar.radian = radian;
		//		vertex.Position = glm::vec3(polar.ConvertToXY(), this->floatPlant[i].x());
		//		ciclePoint.push_back(vertex);
		//		radian += unitRadian;
		//	}
		//	vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
		//	ciclePoint.clear();
		//}

		//flangeTread的indices
		for (int i = 0; i != flangeTreadCircleSize - 1; ++i)//最后一层不用操作
		{
			int currentMuit = section * i;
			int nextMuit = section * (i + 1);
			for (int j = 0; j != section; ++j)
			{
				//正方形第一个三角形
				indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
				indices.push_back(nextMuit + (j + 1) % section); //nextMuit + (j + 1) % section
				indices.push_back(nextMuit + j);//nextMuit + j
				//正方形第二个三角形
				indices.push_back(currentMuit + j);
				indices.push_back(currentMuit + (j + 1) % section);//currentMuit + (j + 1) % section
				indices.push_back(nextMuit + (j + 1) % section);//nextMuit + (j + 1) % section
			}
		}
		size_t flangeTreadIndicesSize = indices.size();

		////floatPlant的indices
		//for (int i = 0; i != floatPlantCircleSize; ++i)
		//{
		//	int currentMuit = section*i + flangeTreadVerticesSize;
		//	int nextMuit = section * (i + 1) + flangeTreadVerticesSize;
		//	if (i != floatPlantCircleSize - 1)
		//	{
		//		for (int j = 0; j != section; ++j)
		//		{
		//			//正方形第一个三角形
		//			indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
		//			indices.push_back(nextMuit + j);
		//			indices.push_back(nextMuit + (j + 1) % section);
		//			//正方形第二个三角形
		//			indices.push_back(currentMuit + j);
		//			indices.push_back(nextMuit + (j + 1) % section);
		//			indices.push_back(currentMuit + (j + 1) % section);
		//		}
		//	}

		//	if (i == 0)//链接flangeTread第一圈
		//	{
		//		for (int j = 0; j != section; ++j)
		//		{
		//			//正方形第一个三角形
		//			indices.push_back(j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
		//			indices.push_back(currentMuit + j);
		//			indices.push_back(currentMuit + (j + 1) % section);
		//			//正方形第二个三角形
		//			indices.push_back(j);
		//			indices.push_back(currentMuit + (j + 1) % section);
		//			indices.push_back((j + 1) % section);
		//		}
		//	}
		//	else if (i == floatPlantCircleSize - 1)//链接tread最后一圈
		//	{
		//		nextMuit = section*(flangeTreadCircleSize - 1);
		//		for (int j = 0; j != section; ++j)
		//		{
		//			//正方形第一个三角形
		//			indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
		//			indices.push_back(nextMuit + j);
		//			indices.push_back(nextMuit + (j + 1) % section);
		//			//正方形第二个三角形
		//			indices.push_back(currentMuit + j);
		//			indices.push_back(nextMuit + (j + 1) % section);
		//			indices.push_back(currentMuit + (j + 1) % section);
		//		}
		//	}
		//}

		//计算flangeTread法向量
		glm::vec3 a, b;
		for (int i = 0; i < flangeTreadIndicesSize; i += 3)
		{
			a = vertices[indices[i + 1]].Position - vertices[indices[i]].Position;
			b = vertices[indices[i + 2]].Position - vertices[indices[i + 1]].Position;

			vertices[indices[i]].Normal += glm::cross(a, b);
			vertices[indices[i + 1]].Normal += glm::cross(a, b);
			vertices[indices[i + 2]].Normal += glm::cross(a, b);
		}

		//计算floatPlant法向量
		/*for (int i = flangeTreadIndicesSize; i < indices.size(); i += 3)
		{
			a = vertices[indices[i + 1]].Position - vertices[indices[i]].Position;
			b = vertices[indices[i + 2]].Position - vertices[indices[i + 1]].Position;

			vertices[indices[i]].Normal += glm::cross(a, b);
			vertices[indices[i + 1]].Normal += glm::cross(a, b);
			vertices[indices[i + 2]].Normal += glm::cross(a, b);
		}*/

		return OpenglObj::OpenglPointMsg(std::move(vertices), std::move(indices));
	}
	QVector<QVector2D> WheelFlangeTread::GetSection(float radian)
	{
		QVector<QVector2D> oneSection;
		if (radian < 0 || radian>2 * kPIR)
		{
			std::cout << "ERROR::WHEEL::GETSECTION::ILLEGAL RADIAN" << std::endl;
		}
		else
		{

			int section = this->flange_tread.begin()->second.size();
			float unitRadian = 2 * kPIR / (float)section;
			int id = radian / unitRadian;

			for (auto i : this->flange_tread)
			{
				oneSection.push_back(QVector2D(i.first, i.second[id].r));
			}
		}
		return oneSection;
	}

	std::vector<glm::vec2> InitFloatPlate(const std::string& filePath)
	{
		std::vector<glm::vec2> floatPlant;

		std::ifstream file;
		std::string directory = filePath;
		file.open(directory);
		if (!file.is_open()) {
			QString error = "ERROR::WHEELDISPLAY::FLOATPLATE::UNABLE TO OPEN FILE,MAY BE THIS FLOAT PLANT TYPE IS UNFINED";
			QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
			return floatPlant;
			//std::cout << "Unable to open file" << std::endl;
		}
		float x, y;
		float realTread;
		file >> realTread;
		float dy = WheelFlangeTread::kStandardTreadRadius - realTread;//把所有的floatplate flange tread都校准到标准的半径上来
		while (file >> x >> y)
		{
			floatPlant.push_back(glm::vec2(x, y + dy));
		}
		file.close();

		return floatPlant;
	}

	OpenglObj::OpenglPointMsg LoadFloatPlantPoint(const std::vector<glm::vec2>& floatPlant)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		int section = WheelFlangeTread::kFloatPlantSection;
		float unitRadian = 2 * WheelFlangeTread::kPIR / (float)section;
		std::vector<Vertex> ciclePoint;
		Vertex vertex;

		//floatPlant转顶点
		size_t floatPlantCircleSize = floatPlant.size();
		Polar polar;
		for (int i = 0; i != floatPlantCircleSize; ++i)
		{
			//std::vector<Vertex> ciclePoint;
			polar.r = floatPlant[i].y;

			vertex.Normal = glm::vec3(0.0f);
			float radian = 0;
			for (int j = 0; j != section; ++j)  //每一圈上的点
			{
				polar.radian = radian;
				vertex.Position = glm::vec3(polar.ConvertToXY(), floatPlant[i].x);
				ciclePoint.push_back(vertex);
				radian += unitRadian;
			}
			vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
			ciclePoint.clear();
		}


		//floatPlant的indices
		for (int i = 0; i != floatPlantCircleSize - 1; ++i)//最后一层不要操作
		{
			int currentMuit = section*i;
			int nextMuit = section * (i + 1);

			for (int j = 0; j != section; ++j)
			{
				//正方形第一个三角形
				indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
				indices.push_back(nextMuit + j);
				indices.push_back(nextMuit + (j + 1) % section);
				//正方形第二个三角形
				indices.push_back(currentMuit + j);
				indices.push_back(nextMuit + (j + 1) % section);
				indices.push_back(currentMuit + (j + 1) % section);
			}
		}

		//计算floatPlant法向量
		glm::vec3 a, b;
		for (int i = 0; i < indices.size(); i += 3)
		{
			a = vertices[indices[i + 1]].Position - vertices[indices[i]].Position;
			b = vertices[indices[i + 2]].Position - vertices[indices[i + 1]].Position;

			vertices[indices[i]].Normal += glm::cross(a, b);
			vertices[indices[i + 1]].Normal += glm::cross(a, b);
			vertices[indices[i + 2]].Normal += glm::cross(a, b);
		}

		return OpenglObj::OpenglPointMsg(std::move(vertices), std::move(indices));
	}

	OpenglObj::OpenglPointMsg LoadConnectionPoint(const std::map<float, std::vector<Polar>>& flangeTread, const std::vector<glm::vec2> & floatPlant)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		int section = flangeTread.begin()->second.size();
		float unitRadian = 2 * WheelFlangeTread::kPIR / (float)section;
		std::vector<Vertex> ciclePoint;
		Vertex vertex;

		//轮缘踏面第一圈的连接点
		std::map<float, std::vector<Polar>>::const_iterator firstCircle = flangeTread.begin();
		vertex.Normal = glm::vec3(0.0f);
		for (int i = 0; i != section; ++i)  //每一圈上的点
		{
			vertex.Position = glm::vec3(firstCircle->second[i].ConvertToXY(), firstCircle->first);
			ciclePoint.push_back(vertex);
		}
		vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
		ciclePoint.clear();

		//浮板第一圈
		size_t floatPlantCircleSize = floatPlant.size();
		Polar polar;

		int firstId = 0;
		polar.r = floatPlant[firstId].y;

		float radian = 0;
		for (int i = 0; i != section; ++i)  
		{
			polar.radian = radian;
			vertex.Position = glm::vec3(polar.ConvertToXY(), floatPlant[firstId].x);
			ciclePoint.push_back(vertex);
			radian += unitRadian;
		}
		vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
		ciclePoint.clear();

		//浮板第二圈
		int lastId = floatPlantCircleSize-1;
		polar.r = floatPlant[lastId].y;

		radian = 0;
		for (int i = 0; i != section; ++i)  
		{
			polar.radian = radian;
			vertex.Position = glm::vec3(polar.ConvertToXY(), floatPlant[lastId].x);
			ciclePoint.push_back(vertex);
			radian += unitRadian;
		}
		vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
		ciclePoint.clear();

		//轮缘踏面最后一圈的连接点
		std::map<float, std::vector<Polar>>::const_reverse_iterator lastCircle = flangeTread.rbegin();
		vertex.Normal = glm::vec3(0.0f);
		for (int i = 0; i != section; ++i)  //每一圈上的点
		{
			vertex.Position = glm::vec3(lastCircle->second[i].ConvertToXY(), lastCircle->first);
			ciclePoint.push_back(vertex);
		}
		vertices.insert(vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
		ciclePoint.clear();

		//连接
		for (int i = 0; i != 4; i+=2)//最后一层不要操作，连接0-1,2-3
		{
			int currentMuit = section*i;
			int nextMuit = section * (i + 1);

			for (int j = 0; j != section; ++j)
			{
				//正方形第一个三角形
				indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
				indices.push_back(nextMuit + j);
				indices.push_back(nextMuit + (j + 1) % section);
				//正方形第二个三角形
				indices.push_back(currentMuit + j);
				indices.push_back(nextMuit + (j + 1) % section);
				indices.push_back(currentMuit + (j + 1) % section);
			}
		}

		//计算连接法向量
		glm::vec3 a, b;
		for (int i = 0; i < indices.size(); i += 3)
		{
			a = vertices[indices[i + 1]].Position - vertices[indices[i]].Position;
			b = vertices[indices[i + 2]].Position - vertices[indices[i + 1]].Position;

			vertices[indices[i]].Normal += glm::cross(a, b);
			vertices[indices[i + 1]].Normal += glm::cross(a, b);
			vertices[indices[i + 2]].Normal += glm::cross(a, b);
		}

		return OpenglObj::OpenglPointMsg(std::move(vertices), std::move(indices));
	}
	//void Wheel::InitStandardTreadData(std::map<float, std::vector<Polar>>& standardDrawData/*,const std::string &type*/)
	//{
	//	for (int plie = 0; plie != STANDARDPILECOUNT; ++plie)
	//	{
	//		float radian = 0;
	//		std::vector<Polar> circlePoint;
	//		while (radian < 2 * PIR)
	//		{
	//			circlePoint.push_back(Polar(standardWheelMsg[plie], radian));
	//			radian += 0.01f;
	//		}
	//		standardDrawData.insert({ plie,circlePoint });
	//	}
	//}
	//void Wheel::InitSurveyTreadData(std::map<float, std::vector<Polar>>& sampleData)
	//{
	//	for (int j = 0; j != ROW; j += 2)
	//	{
	//		float plie = STARTSAMPLEPLIE + j;
	//		std::vector<Polar> circlePolarPoint;
	//		for (int i = 0; i != COLUMN; ++i)
	//		{
	//			float x = sampleSourceMsg[i][j];
	//			float y = sampleSourceMsg[i][j + 1];
	//			if (x == 0 && y == 0)
	//			{
	//				continue;
	//			}
	//			float r = sqrtf(pow(x, 2) + pow(y, 2));
	//			float radian = atan2(y, x);
	//			radian = radian < 0 ? (radian + 2 * PIR) : radian;
	//
	//			sampleSourceMsg[i][j] = r;
	//			sampleSourceMsg[i][j + 1] = radian;
	//			circlePolarPoint.push_back(Polar(r, radian));
	//		}
	//		sort(circlePolarPoint.begin(), circlePolarPoint.end(), [](const Polar& a, const Polar &b) {return a.radian < b.radian; });
	//		sampleData.insert({ plie ,circlePolarPoint });
	//	}
	//}
	////float Wheel::GetAverRadius(float plie, float radian, std::map<float, std::vector<Polar>>& sampleData)
	////{
	////	//std::cout << "???" << std::endl;
	////	//二分查找
	////	const std::vector<Polar>& thisPlie = sampleData[plie];
	////	unsigned int count = thisPlie.size();
	////
	////	int mid, left = 0, right = count - 1;//闭区间[0, n - 1]
	////	while (left < right)
	////	{
	////		mid = left + ((right + 1 - left) >> 1);//向上取整
	////		if (thisPlie[mid].radian < radian) left = mid;
	////		else right = mid - 1;
	////	}
	////
	////	int a, b;
	////	if (thisPlie[left].radian < radian)
	////	{
	////		a = left;
	////		b = (left + 1) % count;  //处理好边界
	////	}
	////	else
	////	{
	////		a = count - 1;
	////		b = 0;
	////	}
	////	
	////	//std::cout << "radian:"<<radian<<" " << std::endl;
	////	return (thisPlie[a].r + thisPlie[b].r) / 2.0f;
	////	
	////}
	////void Wheel::LoadPoint(std::map<float, std::vector<Polar>>& sampleData, std::map<float, std::vector<Polar>>& standardDrawData, std::vector<glm::vec2>& otherData)
	////{
	////	for (int plie = STARTSAMPLEPLIE; plie < ENDSAMPLEPLIE+1; plie += 2)//为stardardDarwData添加采样数据
	////	{
	////		float radian = 0;
	////		int i = 0;
	////		while (radian<2*PIR)
	////		{
	////			Polar position(GetAverRadius(plie, radian, sampleData),radian);
	////			standardDrawData[plie][i] = position;
	////			radian += 0.01f;
	////			++i;
	////		}
	////	}
	////	for (int plie = STARTSAMPLEPLIE + 1; plie < ENDSAMPLEPLIE; plie += 2)//平均采样区间未赋值数据
	////	{
	////		float radian = 0;
	////		int i = 0;
	////		while (radian<2 * PIR)
	////		{
	////			standardDrawData[plie][i].r = (standardDrawData[plie - 1][i].r + standardDrawData[plie + 1][i].r) / 2;
	////			radian += 0.01f;
	////			++i;
	////		}
	////	}
	////
	////
	////
	////	for (int plie = 0; plie != STANDARDPILECOUNT; ++plie)
	////	{
	////		std::vector<Vertex> ciclePoint;
	////		Vertex vertex;
	////		vertex.Normal = glm::vec3(0.0f);
	////		for (int i = 0; i != CIRCLECOUNT; ++i)
	////		{
	////			vertex.Position = glm::vec3(standardDrawData[plie][i].ConvertToXY(), plie);
	////			ciclePoint.push_back(vertex);
	////		}
	////		this->vertices.insert(this->vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
	////	}
	////	size_t treadVerticesSize = this->vertices.size();
	////
	////	size_t otherDataSize = otherData.size();
	////	for (int i = 0; i != otherDataSize; ++i)
	////	{
	////		std::vector<Vertex> ciclePoint;
	////		Polar polar;
	////		polar.r = otherData[i].y;
	////		Vertex vertex;
	////
	////		float radian = 0;
	////		while (radian<2 * PIR)
	////		{
	////			polar.radian = radian;
	////			vertex.Position = glm::vec3(polar.ConvertToXY(), otherData[i].x);
	////			ciclePoint.push_back(vertex);
	////
	////			radian += 0.01f;
	////		}
	////
	////		this->vertices.insert(this->vertices.end(), ciclePoint.begin(), ciclePoint.end());//形成顶点数组
	////	}
	////
	////	for (int i = 0; i != STANDARDPILECOUNT-1; ++i)//最后一层不用操作
	////	{
	////		int currentMuit = CIRCLECOUNT * i;
	////		int nextMuit = CIRCLECOUNT * (i+1);
	////		for (int j = 0; j != CIRCLECOUNT; ++j)
	////		{
	////			//正方形第一个三角形
	////			this->indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
	////			this->indices.push_back(nextMuit + j);
	////			this->indices.push_back(nextMuit + (j + 1) % CIRCLECOUNT);
	////			//正方形第二个三角形
	////			this->indices.push_back(currentMuit + j);
	////			this->indices.push_back(nextMuit + (j + 1)% CIRCLECOUNT);
	////			this->indices.push_back(currentMuit + (j + 1)% CIRCLECOUNT);
	////		}
	////	}
	////	size_t treadIndicesSize = this->indices.size();
	////	
	////
	////
	////	//otherData indices
	////	for (int i = 0; i != otherDataSize; ++i)
	////	{
	////		int currentMuit = CIRCLECOUNT*i+ treadVerticesSize;
	////		int nextMuit = CIRCLECOUNT * (i + 1)+ treadVerticesSize;
	////		if (i != otherDataSize - 1)
	////		{
	////			for (int j = 0; j != CIRCLECOUNT; ++j)
	////			{
	////				//正方形第一个三角形
	////				this->indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
	////				this->indices.push_back(nextMuit + j);
	////				this->indices.push_back(nextMuit + (j + 1) % CIRCLECOUNT);
	////				//正方形第二个三角形
	////				this->indices.push_back(currentMuit + j);
	////				this->indices.push_back(nextMuit + (j + 1) % CIRCLECOUNT);
	////				this->indices.push_back(currentMuit + (j + 1) % CIRCLECOUNT);
	////			}
	////		}
	////
	////		if (i == 0)//链接tread第一圈
	////		{
	////			for (int j = 0; j != CIRCLECOUNT; ++j)
	////			{
	////				//正方形第一个三角形
	////				this->indices.push_back(j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
	////				this->indices.push_back(currentMuit + j);
	////				this->indices.push_back(currentMuit + (j + 1) % CIRCLECOUNT);
	////				//正方形第二个三角形
	////				this->indices.push_back(j);
	////				this->indices.push_back(currentMuit + (j + 1) % CIRCLECOUNT);
	////				this->indices.push_back((j + 1) % CIRCLECOUNT);
	////			}
	////		}
	////		else if (i == otherDataSize - 1)//链接tread最后一圈
	////		{
	////			nextMuit = CIRCLECOUNT*(STANDARDPILECOUNT - 1);
	////			for (int j = 0; j != CIRCLECOUNT; ++j)
	////			{
	////				//正方形第一个三角形
	////				this->indices.push_back(currentMuit + j);  //只需要让其全部按一圈排列就行，无论顺还是逆时针
	////				this->indices.push_back(nextMuit + j);
	////				this->indices.push_back(nextMuit + (j + 1) % CIRCLECOUNT);
	////				//正方形第二个三角形
	////				this->indices.push_back(currentMuit + j);
	////				this->indices.push_back(nextMuit + (j + 1) % CIRCLECOUNT);
	////				this->indices.push_back(currentMuit + (j + 1) % CIRCLECOUNT);
	////			}
	////		}
	////
	////	}
	////
	////	//计算踏面法向量
	////	for (int i = 0; i < treadIndicesSize; i += 3)
	////	{
	////		glm::vec3 a, b;
	////		a = this->vertices[this->indices[i + 1]].Position - this->vertices[this->indices[i]].Position;
	////		b = this->vertices[this->indices[i + 2]].Position - this->vertices[this->indices[i + 1]].Position;
	////
	////		this->vertices[this->indices[i]].Normal += -glm::cross(a, b);
	////		this->vertices[this->indices[i + 1]].Normal += -glm::cross(a, b);
	////		this->vertices[this->indices[i + 2]].Normal += -glm::cross(a, b);
	////	}
	////
	////	//计算other法向量
	////	for (int i = treadIndicesSize; i < this->indices.size(); i += 3)
	////	{
	////		glm::vec3 a, b;
	////		a = this->vertices[this->indices[i + 1]].Position - this->vertices[this->indices[i]].Position;
	////		b = this->vertices[this->indices[i + 2]].Position - this->vertices[this->indices[i + 1]].Position;
	////
	////		this->vertices[this->indices[i]].Normal += glm::cross(a, b);
	////		this->vertices[this->indices[i + 1]].Normal += glm::cross(a, b);
	////		this->vertices[this->indices[i + 2]].Normal += glm::cross(a, b);
	////	}
	////
	////}

	//std::vector<glm::vec3> Wheel::GetPositonArray()
	//{
	//	//this->vertices = new float[486 * 88];
	//
	//}
	//std::vector<unsigned int> Wheel::GetIndicesArray()
	//{
	//
	//}

	WheelFlangeTread::~WheelFlangeTread()
	{
		//delete[] vertices;
		//delete[] indices;
	}

	//const std::vector<Vertex>&  Wheel::GetVertices() const
	//{
	//	return vertices;
	//}
	//const std::vector<unsigned int>& Wheel::GetIndices() const
	//{
	//	return indices;
	//}



	//int m, l = 0, r = averSize - 1;//闭区间[0, averSize - 1]
	//while (l < r)
	//{
	//	m = l + ((r + 1 - l) >> 1);//向上取整
	//	if (averRadius[m].x < width) l = m;
	//	else r = m - 1;
	//}
	//if ((l == averSize - 1 && abs(averRadius[l].x - width) <= 0.0001) || (l != averSize - 1 && averRadius[l].x <= width))
	//{
	//	return l;
	//}
	//else
	//return -1;


}