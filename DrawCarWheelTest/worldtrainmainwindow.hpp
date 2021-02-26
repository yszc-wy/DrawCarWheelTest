#pragma once
#include <QMainWindow>
#include "ui_dlgtrain3d.h"
#include "WheelGroup.h"
class QSlider;
class QAction;
class QWorldOpenglWidget;
class WorldTrainMainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit WorldTrainMainWindow(QWidget *parent = 0);
	~WorldTrainMainWindow();
	void appendAction(QAction *action, int index);
	void appendAction(QAction *action);

    //接口函数
    void ActivateWheel(WheelSide side,int id, int outputId);//设定要激活轮子的位置,和轮子被点击时输出的ID
    void FreezeWheel(WheelSide side, int id);//冻结，也就是取消对轮子的激活
    void FreezeAllWheel();//冻结所有轮子
signals:
    void PickID(int id);//传出信号


	void Init();
//	void TrainFrameVisibleMsg(bool t);
//	void TrainBoxVisibleMsg(bool t);
//	void RotateMsg(bool t);
private slots:
	//void emitTrainFrameVisibleSignal();
	//void emitTrainBoxVisibleSignal();
	//void emitRotateSignal();
	void InitWidget();

	void UpdateActionRotate(bool t);//按下松开。。
	//void updateActionBigger(bool t);//变灰色。。
	//void updateActionSmaller(bool t);
private:
    void createAction();
    void createWidget();

    QAction *actionBigger;
    QAction *actionSmaller;
    QAction *actionRotate;
    QAction *actionRecovery;
    QAction *actionTrainFrameVisible;
    QAction *actionTrainBoxVisible;
    QToolBar *toolBar;
    QSlider *focusSlider;

    QWorldOpenglWidget *worldWidget;

	Ui_DlgTrain3D *ui;
	
};
