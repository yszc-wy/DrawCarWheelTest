﻿#include "worldtrainmainwindow.hpp"
#include "qworldopenglwidget.hpp"
#include <qslider.h>
WorldTrainMainWindow::WorldTrainMainWindow(QWidget *parent) 
	: QMainWindow(parent), ui(new Ui_DlgTrain3D)
{
	ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);

    createWidget();
    createAction();
    ui->toolBar->addAction(actionBigger);
    ui->toolBar->addAction(actionSmaller);
    ui->toolBar->addAction(actionRotate);
    ui->toolBar->addAction(actionRecovery);
    ui->toolBar->addAction(actionTrainBoxVisible);
    ui->toolBar->addAction(actionTrainFrameVisible);
    ui->toolBar->addWidget(focusSlider);
	//connect(worldWidget, SIGNAL(biggerSignal(bool)), this, SLOT(updateActionBigger(bool)));
    //connect(worldWidget, SIGNAL(smallerSignal(bool)), this, SLOT(updateActionSmaller(bool)))

}

void WorldTrainMainWindow::createWidget()
{
    this->worldWidget = new QWorldOpenglWidget(ui->centralwidget);
    connect(this->worldWidget,SIGNAL(PickID(int)),this,SIGNAL(PickID(int)));
    this->setCentralWidget(worldWidget);

    this->focusSlider= new QSlider(Qt::Horizontal);
    this->focusSlider->setMinimum(0);
    this->focusSlider->setMaximum(1000);
    connect(focusSlider, SIGNAL(valueChanged(int)), worldWidget, SLOT(MoveCameraFocus(int)));
    this->focusSlider->setValue(0);

}
void WorldTrainMainWindow::createAction()
{
    actionBigger = new QAction(QIcon(":/images/bigger.png"), "放大");
    connect(actionBigger, SIGNAL(triggered()), worldWidget, SLOT(Bigger()));

    actionSmaller = new QAction(QIcon(":/images/smaller.png"), "缩小");
    connect(actionSmaller, SIGNAL(triggered()), worldWidget, SLOT(Smaller()));

    actionRotate = new QAction(QIcon(":/images/rotate.png"), "旋转");
    actionRotate->setCheckable(true);
    connect(actionRotate, SIGNAL(triggered()), worldWidget, SLOT(Rotate()));
    connect(worldWidget, SIGNAL(rotateSignal(bool)), this, SLOT(UpdateActionRotate(bool)));
    //connect(this, SIGNAL(RotateMsg(bool)), worldWidget, SLOT(Rotate(bool)));

    actionRecovery = new QAction(QIcon(":/images/recoveryDefault.png"), "恢复原图");
    connect(actionRecovery, SIGNAL(triggered()), this, SLOT(InitWidget()));
    connect(this, SIGNAL(Init()), worldWidget, SLOT(InitCamera()));

    actionTrainBoxVisible = new QAction("TrainBox");
    actionTrainBoxVisible->setCheckable(true);
    connect(actionTrainBoxVisible, SIGNAL(triggered()), worldWidget, SLOT(TrainBoxVisible()));
    //connect(this, SIGNAL(TrainBoxVisibleMsg(bool)), worldWidget, SLOT(TrainBoxVisible(bool)));
    actionTrainBoxVisible->setChecked(true);

    actionTrainFrameVisible=new QAction("TrainFrame");
    actionTrainFrameVisible->setCheckable(true);
    connect(actionTrainFrameVisible, SIGNAL(triggered()), worldWidget, SLOT(TrainFrameVisible()));
    //connect(this, SIGNAL(TrainFrameVisibleMsg(bool)), worldWidget, SLOT(TrainFrameVisible(bool)));
    actionTrainFrameVisible->setChecked(true);
}
//void WorldTrainMainWindow::emitTrainFrameVisibleSignal()
//{
//	if (actionTrainFrameVisible->isChecked())
//	{
//		emit TrainFrameVisibleMsg(true);
//	}
//	else
//	{
//		emit TrainFrameVisibleMsg(false);
//	}
//}
//void WorldTrainMainWindow::emitTrainBoxVisibleSignal()
//{
//	if (actionTrainBoxVisible->isChecked())
//	{
//		emit TrainBoxVisibleMsg(true);
//	}
//	else
//	{
//		emit TrainBoxVisibleMsg(false);
//	}
//}
//void WorldTrainMainWindow::emitRotateSignal()
//{
//	if (actionRotate->isChecked())
//	{
//		emit RotateMsg(true);
//	}
//	else
//	{
//		emit RotateMsg(false);
//	}
//}

WorldTrainMainWindow::~WorldTrainMainWindow() {
	
}

void WorldTrainMainWindow::UpdateActionRotate(bool t)
{
	this->actionRotate->setChecked(t);
}
//void WorldTrainMainWindow::updateActionBigger(bool t)
//{
//	this->actionBigger->setEnabled(t);
//}
//void WorldTrainMainWindow::updateActionSmaller(bool t)
//{
//	this->actionSmaller->setEnabled(t);
//}
void WorldTrainMainWindow::appendAction(QAction *action, int index)
{
	ui->toolBar->insertAction(ui->toolBar->actions().at(index), action);
}

void WorldTrainMainWindow::appendAction(QAction *action)
{
	ui->toolBar->addAction(action);
}

void WorldTrainMainWindow::InitWidget()
{
	focusSlider->setValue(0);
	actionTrainBoxVisible->setChecked(true);
	actionTrainFrameVisible->setChecked(true);
	emit Init();
}


void WorldTrainMainWindow::ActivateWheel(WheelSide side,int id, int outputId)//设定要激活轮子的位置,和轮子被点击时输出的ID
{
    this->worldWidget->ActivateWheel(side,id,outputId);
}
void WorldTrainMainWindow::FreezeWheel(WheelSide side, int id)//冻结，也就是取消对轮子的激活
{
    this->FreezeWheel(side,id);
}
void WorldTrainMainWindow::FreezeAllWheel()//冻结所有轮子
{
    this->FreezeAllWheel();
}
