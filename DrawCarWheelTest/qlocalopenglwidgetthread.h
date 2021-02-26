#pragma once
#include <qobject.h>
#include <qmutex.h>
#include <qstring.h>
#include <qvector.h>
#include <qvector2d.h>

#include "FilePath.h"
#include "Wheel.h"

class QLocalOpenGLWidgetThread :public QObject
{
	Q_OBJECT
public:
	QLocalOpenGLWidgetThread(bool context_init_state, QObject* parent = NULL);
	~QLocalOpenGLWidgetThread();
	void set_context_init_state(bool state);
signals:
	void ProgressLog(const QString& info, int present);//½ø¶È
	void ErrorLog(const QString& info);
	void Result(int wheel_id,const OpenglObj::OpenglPointMsg &wheelFlangeTreadMsg, const OpenglObj::OpenglPointMsg& wheelConnectionMsg, const std::string& float_plant_path, bool new_float_plant, const OpenglObj::OpenglPointMsg& floatPlantMsg);
	void Result(int current_wheel_id);
public slots:
	void Run(
		const QString& wheel_id,
		const QVector<QVector2D>& surveyFlange, 
		const QMap<float, QVector<float>>& surveyTread, 
		FilePath::FloatPlant float_plant,
		std::map<std::string, int>* wheel_ids,
		std::vector<Wheel::WheelFlangeTread>* wheel_flange_treads,
		std::vector<Wheel::WheelColorContrast>* wheel_color_contrasts,
		std::map<std::string, std::vector<glm::vec2>>* float_plants
	);
private:
	volatile bool context_is_init;
	QMutex mutex;
};