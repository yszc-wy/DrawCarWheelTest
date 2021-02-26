#pragma once
#include <map>

#include <qopenglwidget.h>
#include <qopenglfunctions_3_3_compatibility.h>
#include <glm\glm.hpp>
#include <qbasictimer.h>
#include <qmap.h>
#include <qvector2d.h>
#include <qvector.h>

#include "Wheel.h"
#include "PickMsg.h"
#include "LocalSpreadEngine.h"
#include "EulerAngleCamera.h"
#include "QuaternionsCamera.h"
#include "ShaderProgram.h"
#include "StandardFlangeTreadMsg.h"
#include "ColorGradientStrip.h"
#include "DxfStandardFlangeTreadMsgMaker.h"

//#include "RoamingCamera.h"
//#include "QuaternionsCamera.h"


class QLocalSpreadOpenGLWidgetThread;
class QThread;
class QLocalSpreadOpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Compatibility {
	Q_OBJECT

public:
	explicit QLocalSpreadOpenglWidget(QWidget * parent = Q_NULLPTR);
	~QLocalSpreadOpenglWidget();

	//拾取联动信号
	//void SetPickWidth(double width);  //接口函数，设定圈数据线位置
	void SetPickRadian(double radian);
	void SetPickAngle(double angle);

	//轮子的添加和切换
	void AddWheelFlangeTread(const Wheel::WheelFlangeTread& wft_msg);

	void SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread);

	void ReplaceStandardWheelMsg(const DxfReader::StandardFlangeTreadMsgMaker& msg_maker);

	void SetColorGamut(float color_gamut);

	void AddTestMsg();
public slots:
	void SetCurrentWheel(int id);//设定当前要显示的轮子
signals:
	//拾取联动信号
	void PickSectionRadian(float radian);//每一个截面
	//void PickWidth(float width);//一圈的数据



	void PickSectionAngle(float angle);

	//按钮响应
	void RotateSignal(bool t);

	//全屏响应
	void FullScreen(bool fs);

	//线程启动
	void StartAddWheelThread(
		const QString& wheel_id,
		const QVector<QVector2D>& surveyFlange,
		const QMap<float, QVector<float>>& surveyTread,
		std::map<std::string, int>* wheel_ids,
		std::vector<Wheel::WheelFlangeTread>* wheel_flange_treads,
		std::vector<Wheel::WheelSpreadParm>* wheel_spread_parms
	);

	//发出线程进度信号
	void ProcessLog(const QString& info, int present);
	void ErrorLog(const QString& info);
private slots:
	//多线程添加轮子
	void AddOpenglMsg(int wheel_id,const Wheel::WheelSpreadMsg& spmsg);
	//按钮响应
	void CoordinateVisible();
	void RulerVisible();
	void ColorMarkVisible();
	void EnableSectionPick();
	void EnableWidthPick();
	void Bigger();
	void Smaller();
	void InitCamera();
	void Rotate();
	//void ColorModel(unsigned char color_model);

protected:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent * e)override;
	void timerEvent(QTimerEvent *e)override;
	void keyPressEvent(QKeyEvent * e)override;
	void keyReleaseEvent(QKeyEvent * e)override;
	void wheelEvent(QWheelEvent * e) override;
	//void showEvent(QShowEvent *e) override;
	void paintEvent(QPaintEvent *e) override;
	void mouseDoubleClickEvent(QMouseEvent*) override; //放大缩小

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
public:
	static const float kYaw;
	static const float kPitch;
	static const float kZoom;
	static const float kNoFullScreenR;
	static const float kFullScreenR;
	static const float kMouseSensitivity;//鼠标灵敏度
	static const float kMiddleMouseSensitivity;
	static const float kWheelSensitivity;
	static const float kCameraFocusOffset;
	static const float kRotateUnitOffset;
	static const float kRoamingThreshold;
	static const float kCameraLocalAngle;
	//color strip
	static const int kColorStripPosition_X;
	static const int kColorStripPosition_Y;
	static const int kColorStripWidth;
	static const int kColorStripHeight;
	static const int kColorStripTextSize;
	static const float kColorStripSize;
	static const float kRoamingOffsetSpeedRatio;
	//color gamut
	static const float kDefaultColorGamut;

	//伸长长度
	static const float kSpreadExtendLength;

	//ruler mark
	static const float kHRulerMarkTextDisToMark;
	static const float kRulerTextSize;
private:

	void ContextIsInit();
	void CreateThread();
	void AddWheelSpread(const Wheel::WheelSpreadMsg& spmsg);

	//void AddDefaultStandardWheelMaker();
	void UpdateStandardFlangeTreadAndColorModelForShader();
	void UpdateColorGamutForColorStripAndShader();
	void UpdateWhenFullScreenOrRecover();

	void Draw2DText(/*QPainter& painter*/);
	void DrawOpengl();

	void LoadFontTexture();
	void LoadExampleMsg();
	void Pick(int posX, int posY);
	void InitShaderValue();
	void InitShaders();
	void InitRuler();
	void InitCartesianCoordinate();
	void InitColorGradientStrip();

	//对于wheel_spread_shader_program的更新
	//void UpdateStandardFlangeTreadAndColorModelForShader();

	ShaderProgram wheel_spread_shader_program;//着色器
	ShaderProgram line_shader_program;//
	ShaderProgram font_shader_program;//
	ShaderProgram _2d_shader_program;

	Engine::LocalSpreadEngine engine;//存储mesh
	Camera::QuaternionsCamera camera;
	//Camera::EulerAngleCamera camera;

	//QuaternionsCamera camera;
	QBasicTimer timer;

	glm::mat4 view;
	glm::mat4 projection;

	//键鼠判断
	bool keyWPressEvent;
	bool keySPressEvent;
	bool keyAPressEvent;
	bool keyDPressEvent;
	float lastX;
	float lastY;
	bool rightButtonDown;
	bool leftButtonDown;
	bool middleButtonDown;
	bool rotateAction;
	bool bigEnable;
	bool smallerEnable;
	bool pickSectionEnable;
	bool pickWidthEnable;
	bool coordinate_visible;
	bool ruler_visible;
	unsigned char color_model;
	bool full_screen_enable;
	bool camera_move_enable;
	bool is_context_init;
	bool camera_roaming_enable;

	float camera_roaming_speed_x;
	float camera_roaming_speed_y;

	PickMsg pickMsg;

	const unsigned int TIMESPAN = 12;

	//const unsigned int SCR_WIDTH = 800;
	//const unsigned int SCR_HEIGHT = 600;
	unsigned int scr_width;
	unsigned int scr_height;

	int currentWheelId;

	std::map<std::string, int> wheel_ids;
	std::vector<Wheel::WheelSpreadParm> wheelSpreadParm;
	std::vector<Wheel::WheelFlangeTread> wheel_flange_treads;
	ColorGradientStrip color_gradient_strip;
	//std::map<std::string, Wheel::StandardFlangeTreadMsgMake> standard_wheel_makers;
	//font_texture
	Texture font_texture;
	//unsigned int font_texture;

	//test
	Object::LineObject coordinate_line;

	//thread
	QLocalSpreadOpenGLWidgetThread* worker;
	QThread* thread;
};