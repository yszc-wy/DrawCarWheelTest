#include "qlocalopenglwidgetthread.h"

#include <qdir.h>

#include "qlocalopenglwidget.hpp"
#include "DxfReader.h"
#include "CadLib\dl_dxf.h"
#include "CadLib\dl_creationadapter.h"
#include "DxfFloatPlantMsgMaker.h"
QLocalOpenGLWidgetThread::QLocalOpenGLWidgetThread(bool context_init_state,QObject* parent)
	:QObject(parent),
	context_is_init(context_init_state)
{

}
QLocalOpenGLWidgetThread::~QLocalOpenGLWidgetThread()
{

}
void QLocalOpenGLWidgetThread::set_context_init_state(bool state)
{
	//std::cout << "QLocalOpenGLWidgetThread::set_context_init_state::start" << std::endl;
	//{
		//QMutexLocker locker(&mutex);
		this->context_is_init = true;
	//}
	//std::cout << "QLocalOpenGLWidgetThread::set_context_init_state::end" << std::endl;
}
void QLocalOpenGLWidgetThread::Run
	(
	const QString& wheel_id,
	const QVector<QVector2D>& surveyFlange,
	const QMap<float, QVector<float>>& surveyTread,
	FilePath::FloatPlant float_plant,
	std::map<std::string, int>* wheel_ids,
	std::vector<Wheel::WheelFlangeTread>* wheel_flange_treads,
	std::vector<Wheel::WheelColorContrast>* wheel_color_contrasts,
	std::map<std::string, std::vector<glm::vec2>>* float_plants
	)
{
	//std::cout << "QLocalOpenGLWidget::SetWheel::3" << std::endl;

	emit ProgressLog("Find wheel id...",0);
	
	if (wheel_ids->find(wheel_id.toStdString()) != wheel_ids->end())
	{
		emit ProgressLog("Success!", 100);
		emit Result((*wheel_ids)[wheel_id.toStdString()]);
		return;
	}

	emit ProgressLog("Loading wheel flange and tread...", 5);
	Wheel::WheelFlangeTread wheel(surveyFlange, surveyTread/*, filePath*/);

	int id;
	{
		QMutexLocker locker(&mutex);
		wheel_flange_treads->clear();
		id = wheel_flange_treads->size();
		wheel_flange_treads->push_back(wheel);
	}

	emit ProgressLog("Adding color contrast...", 10);

	//添加颜色对比

	//QMutexLocker locker(&mutex);
	{
		QMutexLocker locker(&mutex);
		wheel_color_contrasts->clear();//一次性
		wheel_color_contrasts->push_back(Wheel::WheelColorContrast(QLocalOpenGLWidget::kDefaultColorGamut, wheel.get_flange_tread_r_range(), std::vector<glm::vec2>()));
	}

	if (FilePath::GetFloatPlantPath(float_plant) == NULL)
	{
		std::cout << "Error::QLocalOpenGLWidget::SetWheel::illegal float_plant" << std::endl;
		return;
	}

	QDir temDir(FilePath::GetFloatPlantPath(float_plant));
	QString absDir = temDir.absolutePath();
	std::string float_plant_path = absDir.toStdString();

	bool new_float_plant = false;

	OpenglObj::OpenglPointMsg floatPlantMsg;
	if (float_plants->find(float_plant_path) == float_plants->end())//找不到filePath,添加
	{
		std::string error_log = std::string("There are some problem when read this float plant file:\n") + float_plant_path + " :\n";

		emit ProgressLog("Reading float plant .dxf file...", 20);
		DxfReader::DxfReader* reader = new DxfReader::DxfReader();
		DL_Dxf* dxf = new DL_Dxf();
		if (!dxf->in(float_plant_path, reader)) { // if file open failed
			std::cerr << float_plant_path << " could not be opened.\n";
			error_log += std::string("Error::This file could not be opened.\n");
			emit ErrorLog(QString(error_log.c_str()));
			return;
		}

		emit ProgressLog("Loading float plant message maker...", 30);
		auto msg_maker = reader->LoadFPMsgMaker();

		if (reader->get_error_log() != "")//弹出log
		{
			emit ErrorLog(QString((error_log + reader->get_error_log()).c_str()));
			return;
		}

		delete dxf;
		delete reader;

		if (msg_maker.empty())
		{
			std::cout << "QLocalOpenGLWidgetThread::Run::empty msg_maker" << std::endl;
			return;
		}

		emit ProgressLog("Loading float plant basis points...", 40);
		std::vector<glm::vec2> floatPlant = msg_maker.GetFloatPlantPoints(Wheel::WheelFlangeTread::kStandardTreadRadius);


		float_plants->insert({ float_plant_path,floatPlant });


		emit ProgressLog("Loading float plant opengl points...", 50);
		floatPlantMsg = Wheel::LoadFloatPlantPoint(floatPlant);
		new_float_plant = true;
	}

	//添加wheelObject,用表中的第一个作为默认path
	emit ProgressLog("Loading wheel flange and tread opengl points...", 70);
	OpenglObj::OpenglPointMsg wheelFlangeTreadMsg = wheel.LoadFlangeTreadPoint();

	emit ProgressLog("Loading wheel connection points...", 80);
	OpenglObj::OpenglPointMsg wheelConnectionMsg = Wheel::LoadConnectionPoint(wheel.get_flange_tread(), (*float_plants)[float_plant_path]);

	{
		QMutexLocker locker(&mutex);
		wheel_ids->clear();
		wheel_ids->insert({ wheel_id.toStdString(),id });
	}

	emit ProgressLog("Waiting opengl context init...", 90);

	//std::cout << "QLocalOpenGLWidget::SetWheel::4" << std::endl;
	while (1)
	{
		//{
			//QMutexLocker locker(&mutex);
			if (this->context_is_init)
			{
				break;
			}
		//}
	}
	//std::cout << "QLocalOpenGLWidget::SetWheel::5" << std::endl;

	emit ProgressLog("Adding wheel opengl msg...", 95);
	emit Result(id,wheelFlangeTreadMsg, wheelConnectionMsg, float_plant_path, new_float_plant, floatPlantMsg);
	emit ProgressLog("Load Wheel Success!", 100);
}