#include "qlocalspreadopenglwidgetthread.h"

#include "qlocalspreadopenglwidget.hpp"
QLocalSpreadOpenGLWidgetThread::QLocalSpreadOpenGLWidgetThread(bool context_init_state, QObject* parent)
	:
	QObject(parent),
	context_is_init(context_init_state)
{

}
QLocalSpreadOpenGLWidgetThread::~QLocalSpreadOpenGLWidgetThread()
{

}
void QLocalSpreadOpenGLWidgetThread::set_context_init_state(bool state)
{
	//{
		//QMutexLocker locker(&mutex);
		this->context_is_init = state;
	//}
}

void QLocalSpreadOpenGLWidgetThread::Run(
	const QString& wheel_id,
	const QVector<QVector2D>& surveyFlange,
	const QMap<float, QVector<float>>& surveyTread,
	std::map<std::string, int>* wheel_ids,
	std::vector<Wheel::WheelFlangeTread>* wheel_flange_treads,
	std::vector<Wheel::WheelSpreadParm>* wheel_spread_parms
)
{
	//std::cout << "QLocalSpreadOpenGLWidgetThread::Run::1" << std::endl;

	emit ProgressLog("Find wheel id...", 0);

	if (wheel_ids->find(wheel_id.toStdString()) != wheel_ids->end())
	{
		emit ProgressLog("Success!", 100);
		emit Result((*wheel_ids)[wheel_id.toStdString()]);
		return;
	}

	emit ProgressLog("Loading wheel flange and tread...", 30);
	Wheel::WheelFlangeTread wheel(surveyFlange, surveyTread);

	int id;
	{
		QMutexLocker locker(&mutex);
		wheel_flange_treads->clear();
		id = wheel_flange_treads->size();
		//std::cout << "QLocalSpreadOpenGLWidgetThread::Run::id" << std::endl;
		wheel_flange_treads->push_back(wheel);
	}

	emit ProgressLog("Loading wheel spread flange tread opengl msg...", 60);

	Wheel::WheelSpreadMsg spmsg = wheel.LoadSpreadFlangeTreadPoint();

	{
		QMutexLocker locker(&mutex);
		wheel_spread_parms->clear();
		wheel_spread_parms->push_back(spmsg.wheelSpreadParm);
		(*wheel_spread_parms)[wheel_spread_parms->size() - 1].set_color_gamut(QLocalSpreadOpenglWidget::kDefaultColorGamut);//设定默认色域
	}
	//添加id
	{
		QMutexLocker locker(&mutex);
		wheel_spread_parms->clear();
		wheel_ids->insert({ wheel_id.toStdString(),id });
	}

	//std::cout << "QLocalSpreadOpenGLWidgetThread::Run::2" << std::endl;
	emit ProgressLog("Waiting opengl context init...", 90);
	while (1)
	{
		//{
		//	QMutexLocker locker(&mutex);
			if (this->context_is_init)
			{
				break;
			}
		//}
	}
	//std::cout << "QLocalSpreadOpenGLWidgetThread::Run::3" << std::endl;

	emit ProgressLog("Adding wheel opengl msg...", 95);
	emit Result(id,spmsg);
	emit ProgressLog("Load wheel success!", 100);
}