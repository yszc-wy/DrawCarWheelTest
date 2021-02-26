#include <QtWidgets/QApplication>
#include <qlabel.h>
#include <qsurfaceformat.h>
#ifndef  QT_NO_OPENGL
#include <qmetatype.h>


#include "qtwoopenglwidget.hpp"
#include "qlocalmainwindow.h"
#include "qworldtrainmainwindow.hpp"
#include "qlocalspreadmainwindow.hpp"
#include "qtwoopenglmainwindow.hpp"
#include "qtestwidget.h"


#endif // ! QT_NO_OPENGL

void initOpenglVersion();
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Chinese-simplified");

	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<QVector<QVector2D>>("QVector<QVector2D>");
	qRegisterMetaType<QMap<float, QVector<float>>>("QMap<float, QVector<float>>");
	qRegisterMetaType<FilePath::FloatPlant>("FilePath::FloatPlant");
	qRegisterMetaType<std::map<std::string, int>*>("std::map<std::string, int>*");
	qRegisterMetaType<std::vector<Wheel::WheelFlangeTread>*>("std::vector<Wheel::WheelFlangeTread>*");
	qRegisterMetaType<std::vector<Wheel::WheelColorContrast>*>("std::vector<Wheel::WheelColorContrast>*");
	qRegisterMetaType<std::map<std::string, std::vector<glm::vec2>>*>("std::map<std::string, std::vector<glm::vec2>>*");
	qRegisterMetaType<std::vector<Wheel::WheelSpreadParm>*>("std::vector<Wheel::WheelSpreadParm>*");
	qRegisterMetaType<Wheel::WheelSpreadMsg>("Wheel::WheelSpreadMsg");
	qRegisterMetaType<OpenglObj::OpenglPointMsg>("OpenglObj::OpenglPointMsg");


	QApplication app(argc, argv);
	initOpenglVersion();

	app.setApplicationName("cube");
	app.setApplicationVersion("0.1");
#ifndef  QT_NO_OPENGL
	//QTwoOpenglWidget mainWindow;
	//QLocalMainWindow mainWindow;
	//QWorldTrainMainWindow mainWindow;
	//QLocalSpreadMainWindow mainWindow;
	//QTwoOpenglMainWindow mainWindow;
	//mainWindow.show();
	QTestWidget test;
	test.show();
#else // ! QT_NO_OPENGL
	QLabel note("OpenGL Support required");
	note.show();
#endif
	return app.exec();
}

void initOpenglVersion()
{
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	//format.setVersion(4, 3);
	format.setSamples(4);
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	QSurfaceFormat::setDefaultFormat(format);
}