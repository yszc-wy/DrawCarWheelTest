#pragma once
#include <QOpenGLWidget>
//#include <qopenglfunctions_3_3_core.h>
#include <qopenglfunctions_3_3_compatibility.h>
#include <qbasictimer.h>

#include "ShaderProgram.h"
#include "WorldEngine.h"
#include "EulerAngleCamera.h"
//#include "QuaternionsCamera.h"
//#include "RoamingCamera.h"
#include "Train2D.h"

class QWorldOpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Compatibility
{
	Q_OBJECT

public:
	explicit QWorldOpenglWidget(QWidget * parent = Q_NULLPTR);
	~QWorldOpenglWidget();

	void SetMarshalling(int marshalling);
	void SetActivateWheel(WheelSide side, int id);

	//void ActivateWheel(WheelSide side,int id, int outputId);//设定要激活轮子的位置,和轮子被点击时输出的ID
	//void FreezeWheel(WheelSide side, int id);//冻结，也就是取消对轮子的激活
	//void FreezeAllWheel();//冻结所有轮子
signals:
	void PickFrameID(int id);//当被激活的车架被点击时会传出已经设置好的outputId


	//void biggerSignal(bool t);
	//void smallerSignal(bool t);
	void rotateSignal(bool t);


private slots:
	void Bigger();
	void Smaller();
	void InitCamera();
	void Rotate();
	void TrainBoxVisible();
	//void TrainFrameVisible();
	void FrontMoveCameraFocus();
	void BackMoveCameraFocus();

	void MoveCameraFocusBasisOfPercent(int value);//按照百分比移动,废弃
	void MoveCameraFocusBasisOfFrame(int frame_id);
	void MoveCameraFocusBasisOfBox(int box_id);
	void MoveCameraFocusBasisOfMouseOffset(int x_offset);
protected:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent * e)override;
	void timerEvent(QTimerEvent *e)override;
	void wheelEvent(QWheelEvent * e) override;
	void keyPressEvent(QKeyEvent * e) override;
	void keyReleaseEvent(QKeyEvent * e) override;
	
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
public:
	//摄像机常量
	static const float kStartFocus;
	//static const float kEndFocus;
	static const float kFocusYMov;
	static const float kFocusZMov;
	static const float kValueMax;
	static const float kYaw;
	static const float kPitch;
	static const float kZoom;
	static const float kR;
	static const float kRightButtonPressMouseMoveSensitivity;//鼠标灵敏度
	static const float kLeftButtonPressMouseMoveSensitivity;//鼠标左键移动
	static const float kMiddleMouseSensitivity;
	static const float kWheelSensitivity;
	static const float kInitFocus;
	static const float kRotateUnitYaw;
private:
	glm::vec2 GetFrontAndBackFrameId();
	void InitShaders();
	//void InitEngine();
	void InitModel();
	void InitTrain2D();
	void DrawOpengl();
	void Draw2d(QPainter& painter);
	void PickTrain2D(int x, int y);

	int marshalling;

	ShaderProgram shaderProgram;//着色器
	Engine::WorldEngine engine;//存储object

	Camera::EulerAngleCamera camera;
	//QuaternionsCamera camera;
	//RoamingCamera camera;
	QBasicTimer timer;

	glm::mat4 view;
	glm::mat4 projection;

	float lastX;
	float lastY;
	bool rightButtonDown;
	bool leftButtonDown;
	bool middleButtonDown;
	bool rotateAction;
	//bool bigEnable;
	//bool smallerEnable;
	bool trainBoxVisible;
	bool trainFrameVisible;

	bool keyWPressEvent;
	bool keySPressEvent;
	bool keyAPressEvent;
	bool keyDPressEvent;

	const unsigned int TIMESPAN = 12;
	unsigned int scr_width;
	unsigned int scr_height;

	Train2D train_2d;

	Object::MeshObject train_box;
	Object::MeshObject train_head;
	Object::MeshObject train_frame;
	Object::MeshObject train_wheel;
};
