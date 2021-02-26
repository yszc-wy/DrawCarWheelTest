#pragma once

#include <qobject.h>
#include <qmutex.h>
#include <qstring.h>
#include <qvector.h>
#include <qvector2d.h>
#include <qmap.h>

#include "FilePath.h"
#include "Wheel.h"

class QLocalSpreadOpenGLWidgetThread :public QObject 
{
	Q_OBJECT
public:
	QLocalSpreadOpenGLWidgetThread(bool context_init_state, QObject* parent = NULL);
	~QLocalSpreadOpenGLWidgetThread();
	void set_context_init_state(bool state);
signals:
	void ProgressLog(const QString& info, int present);//½ø¶È
	void ErrorLog(const QString& info);
	void Result(int wheel_id, const Wheel::WheelSpreadMsg& spmsg);
	void Result(int current_wheel_id);
public slots:
	void Run(
		const QString& wheel_id,
		const QVector<QVector2D>& surveyFlange,
		const QMap<float, QVector<float>>& surveyTread,
		std::map<std::string, int>* wheel_ids,
		std::vector<Wheel::WheelFlangeTread>* wheel_flange_treads,
		std::vector<Wheel::WheelSpreadParm>* wheel_spread_parms
	);
private:
	volatile bool context_is_init;
	QMutex mutex;
};