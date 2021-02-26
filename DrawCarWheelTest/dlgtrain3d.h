#ifndef DLGTRAIN3D_H
#define DLGTRAIN3D_H


#include <QMainWindow>
#include <qvector2d.h>
namespace Ui {
class DlgTrain3D;
}
class QAction;
class QLocalOpenGLWidget;
class QGoToWidget;
class DlgTrain3D : public QMainWindow
{
    Q_OBJECT

public:
    explicit DlgTrain3D(QWidget *parent = 0);
    ~DlgTrain3D();
    void appendAction(QAction *action, int index);
    void appendAction(QAction *action);

signals:
    //�ӿں���������ź�
    void PickSectionMsg(QVector<QVector2D>);//ÿһ������
    void PickWidth(float width);//һȦ������
private slots:

	void updateActionRotate(bool t);//�����ɿ�����
	//void updateActionBigger(bool t);//���ɫ����
	//void updateActionSmaller(bool t);
private:
    void createAction();
    void createWidget();
    Ui::DlgTrain3D *ui;
    QLocalOpenGLWidget *localWidget;
    QGoToWidget* gotoWidget;

    QAction *actionBigger;
    QAction *actionSmaller;
    QAction *actionRotate;
    QAction *actionRecovery;
    QAction *actionRadianPick;
    QAction *actionCircumferencePick;
    QAction *actionCoordinateAxis;
    QAction *actionRedMark;
    QToolBar *toolBar;
};

#endif // DLGTRAIN3D_H
