#pragma once
#include <qwidget.h>
#include <qmap.h>
#include <qvector.h>
#include <qvector2d.h>

#include "FilePath.h"
class QPushButton;
class QTwoOpenglMainWindow;
class QTestWidget:public QWidget
{
public:
	explicit QTestWidget(QWidget * parent = Q_NULLPTR);
	~QTestWidget();
private slots:
	void ShowTwoOpenglMainWindow();
	void ShowLocalProcessLog(const QString& info, int present);
private:
	void ReadTxtMsg();

	QPushButton *wheel_button0;
	QPushButton *wheel_button1;
	QTwoOpenglMainWindow* main_window;
	
	int id;
	//msg
	QString wheel_id;
	QMap<float, QVector<float>> surveyTread;
	QVector<QVector2D> surveyFlange;
	FilePath::FloatPlant float_plant;
};