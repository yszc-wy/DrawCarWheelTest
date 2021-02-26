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

	//�Ը�����л�
	void SwitchFloatPlant(std::string float_plant_file_path);
	//�Ը���ĸ���
	void UpdateFloatPlant(std::string float_plant_file_path, const DxfReader::FloatPlantMsgMaker& msg_maker);

	void ReplaceStandardWheelMsg(const /*Wheel::StandardFlangeTreadMsgMake*/DxfReader::StandardFlangeTreadMsgMaker& msg_maker);

	void SetColorGamut(float color_gamut);

	//������
	void AddWheel(const Wheel::WheelFlangeTread& wft_msg, std::string type="");

	//��ǰ�ӿ�
	void SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, FilePath::FloatPlant float_plant);

	void SetCurrentWheel(int id);

	//void SetOpenGLContext(QOpenGLContext* context);
protected:
	//void mouseDoubleClickEvent(QMouseEvent* e) override;
signals:
	//�����ź�
	void ProcessLog(const QString& info, int present);
	void ErrorLog(const QString& info);

	void InitGLOver();
    //�ӿں���������ź�
    void PickSectionMsg(QVector<QVector2D>);//ÿһ������
    //void PickWidth(float width);//һȦ������
	void SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg msg);//����չ�����ڵ���������
	void FullScreen(bool fs);
private slots:
	void SetPickRadian(float radian);
	void SetPickAngle(float angle);

	void EnableActions(bool full_screen_enable);
	void updateActionRotate(bool t);//�����ɿ�����
	//void updateActionBigger(bool t);//���ɫ����
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
