#include "qlocalspreadmainwindow.hpp"

#include <QAction>
#include <QToolBar>
#include <QLayout>
#include <QSplitter>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qstringlist.h>
#include <qevent.h>

#include "qlocalspreadopenglwidget.hpp"
#include "ui_dlgtrain3d.h"
QLocalSpreadMainWindow::QLocalSpreadMainWindow(QWidget * parent) 
	: QMainWindow(parent), 
	//full_screen(false),
		//file_dialog(NULL),
	 ui(new Ui_DlgTrain3D)
{
	ui->setupUi(this);
	this->setCentralWidget(ui->centralwidget);
	setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);

	this->createWidget();

	this->createAction();

	//connect(localSpreadWidget, SIGNAL(PickWidth(float)), this, SIGNAL(PickWidth(float)));//?

	connect(localSpreadWidget, SIGNAL(PickSectionAngle(float)), this, SIGNAL(PickSectionAngle(float)));

	connect(localSpreadWidget, SIGNAL(PickSectionRadian(float)), this, SIGNAL(PickSectionRadian(float)));

	connect(localSpreadWidget, SIGNAL(FullScreen(bool)), this, SIGNAL(FullScreen(bool)));

	connect(localSpreadWidget, SIGNAL(FullScreen(bool)), this, SLOT(EnableActions(bool)));

	connect(localSpreadWidget, &QLocalSpreadOpenglWidget::ProcessLog, this, &QLocalSpreadMainWindow::ProcessLog);

	connect(localSpreadWidget, &QLocalSpreadOpenglWidget::ErrorLog, this, &QLocalSpreadMainWindow::ErrorLog);
}
void QLocalSpreadMainWindow::createWidget()
{
	this->localSpreadWidget = new QLocalSpreadOpenglWidget(ui->centralwidget);
	this->setCentralWidget(localSpreadWidget);

	////create sft import
	//this->combobox_label=new QLabel("对比踏面类型:",this);
	//this->model_combobox=new QComboBox(this);
	//this->model_combobox->addItem("自身高度");
	//this->model_combobox->addItem("SH4L-SY标准轮");
	//this->add_button=new QPushButton("添加标准轮数据");

	//connect(this->add_button, SIGNAL(clicked()), this, SLOT(AddSFTText()));

	//ui->toolBar->addWidget(this->combobox_label);
	//ui->toolBar->addWidget(this->model_combobox);
	//ui->toolBar->addWidget(this->add_button);
}
void QLocalSpreadMainWindow::createAction()
{
	actionBigger = new QAction("Bigger"); 
	actionBigger->setEnabled(false);
	connect(actionBigger, SIGNAL(triggered()), localSpreadWidget, SLOT(Bigger()));

	actionSmaller = new QAction("Smaller");
	actionSmaller->setEnabled(false);
	connect(actionSmaller, SIGNAL(triggered()), localSpreadWidget, SLOT(Smaller()));

	actionRotate = new QAction("Rotate");
	//actionRotate->setCheckable(true);
	actionRotate->setEnabled(false);
	connect(actionRotate, SIGNAL(triggered()), localSpreadWidget, SLOT(Rotate()));
	//connect(localSpreadWidget, SIGNAL(RotateSignal(bool)), this, SLOT(updateActionRotate(bool)));

	actionRecovery = new QAction("Recovery");
	actionRecovery->setEnabled(false);
	connect(actionRecovery, SIGNAL(triggered()), localSpreadWidget, SLOT(InitCamera()));

	/*actionSectionPick = new QAction("SectionPickEnable");
	actionSectionPick->setCheckable(true);
	connect(actionSectionPick, SIGNAL(triggered()), localSpreadWidget, SLOT(EnableSectionPick()));
	actionSectionPick->setChecked(true);

	actionCircumferencePick = new QAction("WidthPickEnable");
	actionCircumferencePick->setCheckable(true);
	connect(actionCircumferencePick, SIGNAL(triggered()), localSpreadWidget, SLOT(EnableWidthPick()));
	actionCircumferencePick->setChecked(true);*/

	//actionColorMark = new QAction("ColorMarkVisible");
	//actionColorMark->setCheckable(true);
	//connect(actionColorMark, SIGNAL(triggered()), localSpreadWidget, SLOT(ColorMarkVisible()));
	//actionColorMark->setChecked(true);

	//actionCoordinate = new QAction("CoordinateVisible");
	//actionCoordinate->setCheckable(true);
	//connect(actionCoordinate, SIGNAL(triggered()), localSpreadWidget, SLOT(CoordinateVisible()));
	//actionCoordinate->setChecked(true);

	//actionRuler = new QAction("RulerVisible");
	//actionRuler->setCheckable(true);
	//connect(actionRuler, SIGNAL(triggered()), localSpreadWidget, SLOT(RulerVisible()));
	//actionRuler->setChecked(true);
	//actionRuler

	ui->toolBar->addAction(actionBigger);
	ui->toolBar->addAction(actionSmaller);
	ui->toolBar->addAction(actionRotate);
	ui->toolBar->addAction(actionRecovery);
	//ui->toolBar->addAction(actionSectionPick);
	//ui->toolBar->addAction(actionCircumferencePick);
	//ui->toolBar->addAction(actionColorMark);
	//ui->toolBar->addAction(actionRuler);
	//ui->toolBar->addAction(actionCoordinate);
}

QLocalSpreadMainWindow::~QLocalSpreadMainWindow() {
	delete ui;
}

void QLocalSpreadMainWindow::updateActionRotate(bool t)
{
	this->actionRotate->setChecked(t);
}

void QLocalSpreadMainWindow::appendAction(QAction *action, int index)
{
	ui->toolBar->insertAction(ui->toolBar->actions().at(index), action);
}

void QLocalSpreadMainWindow::appendAction(QAction *action)
{
	ui->toolBar->addAction(action);
}

void QLocalSpreadMainWindow::SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread)
{
	this->localSpreadWidget->SetWheel(wheel_id, surveyFlange, surveyTread);
}

void QLocalSpreadMainWindow::AddTestMsg()
{
	this->localSpreadWidget->AddTestMsg();
}
//接收轮子信息
//void QLocalSpreadMainWindow::AddWheelSpread(Wheel::WheelSpreadMsg msg)
//{
//	//this->localSpreadWidget-(msg);
//}

//void QLocalSpreadMainWindow::SetCurrentWheel(int id)
//{
//	this->localSpreadWidget->SetCurrentWheel(id);
//}

//void QLocalSpreadMainWindow::SetPickWidth(double width)
//{
//	this->localSpreadWidget->SetPickWidth(width);
//}
void QLocalSpreadMainWindow::SetPickRadian(double radian)
{
	this->localSpreadWidget->SetPickRadian(radian);
}
void QLocalSpreadMainWindow::SetPickAngle(double angle)
{
	this->localSpreadWidget->SetPickAngle(angle);
}

void QLocalSpreadMainWindow::ReplaceStandardWheelMsg(const DxfReader::StandardFlangeTreadMsgMaker& msg_maker)
{
	this->localSpreadWidget->ReplaceStandardWheelMsg(msg_maker);
}

void QLocalSpreadMainWindow::SetColorGamut(float color_gamut)
{
	this->localSpreadWidget->SetColorGamut(color_gamut);
}

void QLocalSpreadMainWindow::AddWheel(const Wheel::WheelFlangeTread& wft_msg)
{
	this->localSpreadWidget->AddWheelFlangeTread(wft_msg);
}

void QLocalSpreadMainWindow::SetCurrentWheel(int id)
{
	this->localSpreadWidget->SetCurrentWheel(id);
}
//void QLocalSpreadMainWindow::mouseDoubleClickEvent(QMouseEvent* e)
//{
//	if (e->button() == Qt::LeftButton)
//	{
//		if (this->full_screen == false)
//		{
//			emit FullScreen(true);
//			this->full_screen = true;
//		}
//		else
//		{
//			/*this->setWindowFlags(Qt::SubWindow);
//			this->showNormal();*/
//			emit FullScreen(false);
//			this->full_screen = false;
//		}
//	}
//}

void QLocalSpreadMainWindow::EnableActions(bool full_screen_enable)
{
	this->actionBigger->setEnabled(full_screen_enable);
	this->actionSmaller->setEnabled(full_screen_enable);
	this->actionRotate->setEnabled(full_screen_enable);
	this->actionRecovery->setEnabled(full_screen_enable);
}

