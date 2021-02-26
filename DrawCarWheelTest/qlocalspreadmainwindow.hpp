#pragma once
#include <QMainWindow>


#include "Wheel.h"
#include "DxfStandardFlangeTreadMsgMaker.h"

class QComboBox;
class QLabel;
class QPushButton;
class Ui_DlgTrain3D;
class QLocalSpreadOpenglWidget;
class QFileDialog;
class QLocalSpreadMainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit QLocalSpreadMainWindow(QWidget * parent = Q_NULLPTR);
	~QLocalSpreadMainWindow();
	void appendAction(QAction *action, int index);
	void appendAction(QAction *action);

	void ReplaceStandardWheelMsg(const /*Wheel::StandardFlangeTreadMsgMake*/DxfReader::StandardFlangeTreadMsgMaker& msg_maker);

	void SetColorGamut(float color_gamut);

	void AddWheel(const Wheel::WheelFlangeTread& wft_msg);

	void SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread);

	void SetCurrentWheel(int id);

protected:
	//void mouseDoubleClickEvent(QMouseEvent*) override;
signals:

	//接口函数，输出信号
	void PickSectionRadian(float radian);
	void PickSectionAngle(float angle);//每一个截面
	void FullScreen(bool fs);
	//void PickWidth(float width);//一圈的数据

	//线程信号
	void ProcessLog(const QString& info, int present);
	void ErrorLog(const QString& info);

private slots:
	void AddTestMsg();
	//主窗口接收轮子信息
	//void AddWheelSpread(Wheel::WheelSpreadMsg msg);
	//void SetCurrentWheel(int id);//设定当前要显示的轮子

	//void SetPickWidth(double width);  
	void SetPickRadian(double radian);
	void SetPickAngle(double angle);

	void EnableActions(bool full_screen_enable);
	//内部响应
	void updateActionRotate(bool t);
	//void AddSFTText();
private:
	void createAction();
	void createWidget();

	//bool full_screen;

	Ui_DlgTrain3D *ui;
	QLocalSpreadOpenglWidget *localSpreadWidget;
	QAction *actionBigger;
	QAction *actionSmaller;
	QAction *actionRotate;
	QAction *actionRecovery;
	QAction *actionSectionPick;
	QAction *actionCircumferencePick;
	QAction *actionColorMark;
	QAction *actionCoordinate;
	QAction *actionRuler;
	QToolBar *toolBar;

	//QFileDialog *file_dialog;
};
