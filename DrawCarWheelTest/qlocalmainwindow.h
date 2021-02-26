#ifndef DLGTRAIN3D_H
#define DLGTRAIN3D_H

#include <QMainWindow>
#include <qvector2d.h>

#include "Wheel.h"

#include "DxfStandardFlangeTreadMsgMaker.h"
#include "DxfFloatPlantMsgMaker.h"
#include "FilePath.h"
namespace Ui {
class DlgTrain3D;
}
class QAction;
class QLocalOpenGLWidget;
class QOpenGLContext;
//class QGoToWidget;
class QLocalMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QLocalMainWindow(QWidget *parent = 0);
    ~QLocalMainWindow();
    void appendAction(QAction *action, int index);
    void appendAction(QAction *action);

	//对浮板的切换
	void SwitchFloatPlant(std::string float_plant_file_path);
	//对浮板的更新
	void UpdateFloatPlant(std::string float_plant_file_path, const DxfReader::FloatPlantMsgMaker& msg_maker);

	void ReplaceStandardWheelMsg(const /*Wheel::StandardFlangeTreadMsgMake*/DxfReader::StandardFlangeTreadMsgMaker& msg_maker);

	void SetColorGamut(float color_gamut);

	//待废弃
	void AddWheel(const Wheel::WheelFlangeTread& wft_msg, std::string type="");

	//当前接口
	void SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, FilePath::FloatPlant float_plant);

	void SetCurrentWheel(int id);

	//void SetOpenGLContext(QOpenGLContext* context);
protected:
	//void mouseDoubleClickEvent(QMouseEvent* e) override;
signals:
	//进度信号
	void ProcessLog(const QString& info, int present);
	void ErrorLog(const QString& info);

	void InitGLOver();
    //接口函数，输出信号
    void PickSectionMsg(QVector<QVector2D>);//每一个截面
    //void PickWidth(float width);//一圈的数据
	void SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg msg);//发给展开窗口的轮子数据
	void FullScreen(bool fs);
private slots:
	void SetPickRadian(float radian);
	void SetPickAngle(float angle);

	void EnableActions(bool full_screen_enable);
	void updateActionRotate(bool t);//按下松开。。
	//void updateActionBigger(bool t);//变灰色。。
	//void updateActionSmaller(bool t);
private:
    void createAction();
    void createWidget();

	//bool full_screen;

    Ui::DlgTrain3D *ui;
    QLocalOpenGLWidget *localWidget;
    //QGoToWidget* gotoWidget;
    QAction *actionBigger;
    QAction *actionSmaller;
    QAction *actionRotate;
    QAction *actionRecovery;
    QAction *actionRadianPick;
    QAction *actionCircumferencePick;
  //  QAction *actionCoordinateAxis;
    QAction *actionColorMark;
    QToolBar *toolBar;

	
};

#endif // DLGTRAIN3D_H
