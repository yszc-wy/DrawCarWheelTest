#include "qtestwidget.h"

#include <fstream>

#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlayout.h>

#include "qtwoopenglmainwindow.hpp"

QTestWidget::QTestWidget(QWidget * parent)
	:QWidget(parent),
	wheel_button0(nullptr),
	wheel_button1(nullptr),
	main_window(nullptr),
	id(-1),
	wheel_id(""),
	surveyTread(),
	surveyFlange(),
	float_plant(FilePath::FloatPlant::SH840_135)
{
	wheel_button0 = new QPushButton("wheel0",this);
	wheel_button1 = new QPushButton("wheel1", this);

	connect(this->wheel_button0, &QPushButton::clicked, this, &QTestWidget::ShowTwoOpenglMainWindow);
	connect(this->wheel_button1, &QPushButton::clicked, this, &QTestWidget::ShowTwoOpenglMainWindow);
		//std::cout << "What?" << std::endl;

	QVBoxLayout *main_layout = new QVBoxLayout();
	main_layout->addWidget(this->wheel_button0);
	main_layout->addWidget(this->wheel_button1);

	this->setLayout(main_layout);
}
void QTestWidget::ShowLocalProcessLog(const QString& info, int present)
{
	//std::cout << info.toStdString() << std::endl;
}
QTestWidget::~QTestWidget()
{

}
void QTestWidget::ShowTwoOpenglMainWindow()
{
	this->ReadTxtMsg();

	//QTwoOpenglMainWindow *main_window=new QTwoOpenglMainWindow(this);
	//main_window->setAttribute(Qt::WA_DeleteOnClose, true);
	//connect(main_window, &QTwoOpenglMainWindow::LocalProcessLog, this, &QTestWidget::ShowLocalProcessLog);
	//main_window->show();
	//main_window->SetWheel(this->wheel_id, this->surveyFlange, this->surveyTread, this->float_plant);

	if (this->main_window == nullptr)
	{
		this->main_window = new QTwoOpenglMainWindow(this);
		connect(this->main_window, &QTwoOpenglMainWindow::LocalProcessLog, this, &QTestWidget::ShowLocalProcessLog);
	}
	this->main_window->show();
	this->main_window->SetWheel(this->wheel_id, this->surveyFlange, this->surveyTread, this->float_plant);
}
void QTestWidget::ReadTxtMsg()
{
	//QMap<float, QVector<float>> surveyTread;
	this->surveyTread.clear();
	this->surveyFlange.clear();

	this->wheel_id = QString("%1").arg(++this->id);

	std::ifstream file;
	file.open(FilePath::kSurveyTreadPath);
	if (!file.is_open()) {
		QString error = "ERROR::QLocalOpenGLWidget::ReadTxtMsg::UNABLE TO OPEN FILE Tread";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	}
	float y;
	QVector<float> circlePoint;
	float startX = 50.0f;
	float cicleSize = 2335;
	int i = 0;
	int j = 0;
	while (file >> y)
	{
		circlePoint.push_back(y);
		++i;
		if (i % 2335 == 0)
		{
			this->surveyTread.insert(startX + 0.1f*j, circlePoint);
			circlePoint.clear();
			++j;
		}
	}
	file.close();

	//QVector<QVector2D> surveyFlange;
	file.open(FilePath::kSurveyFlangePath);
	if (!file.is_open()) {
		QString error = "ERROR::QLocalOpenGLWidget::ReadTxtMsg::UNABLE TO OPEN FILE Flange";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	}
	float x;
	while (file >> x >> y)
	{
		this->surveyFlange.push_back(QVector2D(x, y));
	}
	file.close();

	//this->AddTestWheel(surveyFlange, surveyTread, FilePath::k_FloatPlant_SH840_4L_CadPath);

}