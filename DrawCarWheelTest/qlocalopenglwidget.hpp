//#ifndef QLOCALOPENGLWIDGET_H
//#define QLOCALOPENGLWIDGET_H
#pragma once
#include <qopenglwidget.h>
//#include <qopenglfunctions_3_3_core.h>
#include <qopenglfunctions_3_3_compatibility.h>
#include <qvector2d.h>
#include <qmap.h>
#include <qvector.h>
#include <glm\glm.hpp>
#include <qpainter.h>
#include <qthread.h>

//#include "EulerAngleCamera.h"
//#include "RoamingCamera.h"
#include "QuaternionsCamera.h"
#include "qbasictimer.h"
#include "Wheel.h"
#include "PickMsg.h"
#include "LocalEngine.h"
#include "ShaderProgram.h"
#include "DxfFloatPlantMsgMaker.h"
#include "FilePath.h"
class GeometryEngine;
class QOpenGLTexture;
class QOpenGLContext;
class QLocalOpenGLWidgetThread;
class QLocalOpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions_3_3_Compatibility {
	Q_OBJECT

public:
	explicit QLocalOpenGLWidget(QWidget * parent = Q_NULLPTR);
	~QLocalOpenGLWidget();

	//接口函数
	//待废弃
	void AddWheel(const Wheel::WheelFlangeTread& wft_msg, std::string default_float_plant_file_path = "");
	//添加Wheel，添加后currentId默认为0
	void SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, FilePath::FloatPlant float_plant);

	int GetWheelSize();//获取已经录入的轮子数量
	void ReadTxtMsg();//读取txt测试数据的函数

	//对浮板的切换
	void SwitchFloatPlant(std::string float_plant_file_path);
	//对浮板的更新
	void UpdateFloatPlant(std::string float_plant_file_path,const DxfReader::FloatPlantMsgMaker& msg_maker);

	//mainwindow
	void SetPickRadian(float radian);
	void SetPickAngle(float angle);

	//颜色模式改变
	void ReplaceStandardWheelMsg(const DxfReader::StandardFlangeTreadMsgMaker& msg_maker);

	//设定gamut
	void SetColorGamut(float color_gamut);
public slots:
	void SetCurrentWheel(int id);//设定当前要显示的轮子
	//设定opengl上下文
	//void SetOpenGLContext(QOpenGLContext *context);
signals:
	//
	void InitGLOver();
	//接口函数，输出信号
	void PickSectionMsg(QVector<QVector2D>);//每一个截面
    //void PickWidth(float width);//一圈的数据
    void PickSectionAngle(float angle);
    //void PickSectionRadian(float radian);
    //void PickWidthRange(double min, double max);
	void SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg msg);
	//按钮响应
	//void BiggerSignal(bool t);
	//void SmallerSignal(bool t);
	void RotateSignal(bool t);

	//全屏响应
	void FullScreen(bool fs);

	//通过信号启动线程
	//注意init
	void StartAddWheelThread(
		const QString& wheel_id,
		const QVector<QVector2D>& surveyFlange,
		const QMap<float, QVector<float>>& surveyTread,
		FilePath::FloatPlant float_plant,
		std::map<std::string, int>* wheel_ids,
		std::vector<Wheel::WheelFlangeTread>* wheel_flange_treads,
		std::vector<Wheel::WheelColorContrast>* wheel_color_contrasts,
		std::map<std::string, std::vector<glm::vec2>>* float_plants
	);
	//发出线程进度信号
	void ProcessLog(const QString& info, int present);
	void ErrorLog(const QString& info);
private slots:
   // void SetPickWidth(double width);  //接口函数，设定圈数据线位置

	//按钮响应
	void EnableSectionPick();
    void EnableWidthPick();
    //void CoordinateAxisVisible();
    void ColorMarkVisible();
	void Bigger();
	void Smaller();
	void InitCamera();
	void Rotate();

	//接收线程信息
	void AddOpenglMsg(int wheel_id, const OpenglObj::OpenglPointMsg &wheelFlangeTreadMsg, const OpenglObj::OpenglPointMsg& wheelConnectionMsg, const std::string& float_plant_path, bool new_float_plant, const OpenglObj::OpenglPointMsg& floatPlantMsg);
protected:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	//void resizeEvent(QResizeEvent *e)override;
	void mouseMoveEvent(QMouseEvent * e)override;
	void timerEvent(QTimerEvent *e)override;
	void keyPressEvent(QKeyEvent * e)override;
	void keyReleaseEvent(QKeyEvent * e)override;
	void wheelEvent(QWheelEvent * e) override;
	//void showEvent(QShowEvent *e) override;
	//void enterEvent(QEvent *event) override;
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
	static const float kCameraLocalAngle;
	static const float kRotateUnitOffset;
	//color gamut
	static const float kDefaultColorGamut;

	//camera 
private:
	void ContextIsInit();
	void CreateThread();

	void AddTestWheel(const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, const std::string& default_float_plant_file_path);//添加Wheel，添加后currentId默认为0
	//对于wheel_spread_shader_program的更新
	void UpdateStandardFlangeTreadAndColorModelForShader();
	void UpdateColorGamutForShader();
	//void AddDefaultStandardWheelMaker();

	void UpdateWheelFlangeTreadModel(float radian);
    void Pick(int posX,int posY);
	void InitShaderValue();
    void InitShaders();
    void InitCoordinateAxis();
	void InitPolarCoordinate();
	void InitCartesianCoordinate();
	void LoadFontTexture();
	void DrawOpengl();
	void Draw2DText(QPainter& painter);
	//void initEngine();

	ShaderProgram shaderProgram_other;//着色器
	ShaderProgram shaderProgram_flange_tread;
	ShaderProgram line_shader_program;//
	ShaderProgram font_shader_program;//

	Engine::LocalEngine engine;//存储mesh
	//RoamingCamera camera;
	//EulerAngleCamera camera;
	Camera::QuaternionsCamera camera;
	QBasicTimer timer;
	//GeometryEngine *geometries;

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
    //bool coordinateAxisVisible;
    bool colorMarkVisible;
	bool full_screen_enable;
	bool camera_move_enable;
	bool is_context_init;

	PickMsg pickMsg;

	const unsigned int TIMESPAN=12;

	//const unsigned int SCR_WIDTH = 800;
	//const unsigned int SCR_HEIGHT = 600;
	unsigned int scr_width;
	unsigned int scr_height;

	int currentWheelId;
	std::map<std::string, std::vector<glm::vec2>> floatPlants; 
	std::map<std::string, int> wheel_ids;
	std::vector<Wheel::WheelFlangeTread> wheel_flange_treads;
	std::vector<Wheel::WheelColorContrast> wheel_color_contrasts;
	//std::map<std::string, Wheel::StandardFlangeTreadMsgMake> standard_wheel_makers;

	//font_texture
	Texture font_texture;

	//QOpenGLContext *context;
	//QSurface *surface;

	QLocalOpenGLWidgetThread *worker;
	QThread *thread;
};
