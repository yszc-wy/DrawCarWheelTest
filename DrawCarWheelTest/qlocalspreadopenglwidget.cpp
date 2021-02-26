#include "qlocalspreadopenglwidget.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>
#include <qmessagebox.h>
#include <qevent.h>
#include <qpainter.h>
#include <qthread.h>

#include "Vertex.h"
#include "Mesh.h"
#include "LoadModel.h"
#include "FilePath.h"
#include "Ruler.h"
#include "stb_image.h"
#include "Font.h"
#include "CoordinateObject.h"
#include "CartesianCoordinate.h"
#include "qlocalspreadopenglwidgetthread.h"


const float QLocalSpreadOpenglWidget::kYaw = -90.0f;
const float QLocalSpreadOpenglWidget::kPitch = -89.9f;
const float QLocalSpreadOpenglWidget::kZoom = 45.0f;
const float QLocalSpreadOpenglWidget::kNoFullScreenR = 4.5f;
const float QLocalSpreadOpenglWidget::kFullScreenR = 4.5f;
const float QLocalSpreadOpenglWidget::kCameraFocusOffset = -50.0f;
const float QLocalSpreadOpenglWidget::kMouseSensitivity = 0.002f;//鼠标灵敏度
const float QLocalSpreadOpenglWidget::kMiddleMouseSensitivity = 0.03f;
const float QLocalSpreadOpenglWidget::kWheelSensitivity = 0.001f;
const float QLocalSpreadOpenglWidget::kRotateUnitOffset = 15.0f;//选择程度
const float QLocalSpreadOpenglWidget::kRoamingThreshold = 5.0f;
const float QLocalSpreadOpenglWidget::kRoamingOffsetSpeedRatio = 0.1f;
const float QLocalSpreadOpenglWidget::kCameraLocalAngle = 0.0f;//摄像机自身角度
//颜色条参数
const int QLocalSpreadOpenglWidget::kColorStripPosition_X = 10;
const int QLocalSpreadOpenglWidget::kColorStripPosition_Y = 20;
const float QLocalSpreadOpenglWidget::kColorStripSize = 1.0f;  //1倍
const int QLocalSpreadOpenglWidget::kColorStripWidth = 20;
const int QLocalSpreadOpenglWidget::kColorStripHeight = 200;
const int QLocalSpreadOpenglWidget::kColorStripTextSize = 7;

//色域
const float QLocalSpreadOpenglWidget::kDefaultColorGamut = 0.75;

//伸长长度
const float QLocalSpreadOpenglWidget::kSpreadExtendLength = 1500.0f;

//ruler参数
const float QLocalSpreadOpenglWidget::kHRulerMarkTextDisToMark = 10.0f;
const float QLocalSpreadOpenglWidget::kRulerTextSize = 8.0f;

QLocalSpreadOpenglWidget::QLocalSpreadOpenglWidget(QWidget * parent)
	: QOpenGLWidget(parent),
	leftButtonDown(false),
	rightButtonDown(false),
	rotateAction(false),
	middleButtonDown(false),
	bigEnable(true),
	smallerEnable(true),
	pickSectionEnable(true),
	pickWidthEnable(true),
	currentWheelId(-1),
	keyWPressEvent(false),
	keySPressEvent(false),
	keyAPressEvent(false),
	keyDPressEvent(false),
	pickMsg(-1.0f, 70.0f),
	coordinate_visible(false),
	full_screen_enable(false),
	ruler_visible(true),
	camera_move_enable(false),
	camera_roaming_enable(false),
	camera_roaming_speed_x(0.0f),
	camera_roaming_speed_y(0.0f),
	is_context_init(false),
	color_model(Wheel::kHeightColorModel),
	worker(nullptr),
	thread(nullptr)
{

	this->scr_width = this->size().width();
	this->scr_height = this->size().height();

	//this->startTimer(TIMESPAN);//ms
	this->timer.start(TIMESPAN, this);
}

QLocalSpreadOpenglWidget::~QLocalSpreadOpenglWidget() { //这个函数要修改上下文，所以需要makeCurrent
	this->makeCurrent();

	this->doneCurrent();
}
void QLocalSpreadOpenglWidget::InitShaders()
{
	wheel_spread_shader_program.initShaderProgram(FilePath::kVWheelSpreadShaderPath,
		FilePath::kFWheelSpreadShaderPath);
	line_shader_program.initShaderProgram(FilePath::kVLineShaderPath, FilePath::kFLineShaderPath);

	//std::cout << "Font Shader!" << std::endl;
	font_shader_program.initShaderProgram(FilePath::kVFontShaderPath, FilePath::kFFontShaderPath);

	_2d_shader_program.initShaderProgram(FilePath::kV2dShaderPath,FilePath::kF2dShaderPath);
}
void QLocalSpreadOpenglWidget::initializeGL()
{
	initializeOpenGLFunctions();//glad

	glViewport(0, 0, scr_width, scr_height);

	// Enable depth buffer
	//glEnable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEBUG_OUTPUT);
	//glDisable(GL_DEPTH_TEST);
	// Enable back face culling
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	this->InitShaders();

	this->LoadFontTexture();
	//Font::LoadFontTexture();
	//this->camera.SetDisToFocus(2.0f);
	this->InitRuler();
	this->InitCartesianCoordinate();
	//this->AddDefaultStandardWheelMaker();//添加默认s
	//this->LoadExampleMsg();

	//this->InitColorGradientStrip();

	this->InitCamera();

	this->InitShaderValue();

	this->ContextIsInit();
}
void QLocalSpreadOpenglWidget::LoadFontTexture()//仅用与initGL使用
{
	this->font_texture = Font::LoadFontTexture();
	font_shader_program.use();
	font_shader_program.setInt("font_texture", 0);//GL_TEXTURE0
}
void QLocalSpreadOpenglWidget::LoadExampleMsg()
{
	QMap<float, QVector<float>> surveyTread;

	std::ifstream file;
	file.open(FilePath::kSurveyTreadPath);
	if (!file.is_open()) {
		QString error = "ERROR::QLocalOpenGLWidget::ReadTxtMsg::UNABLE TO OPEN FILE Tread";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	}
	float y;
	QVector<float> circlePoint;
	float startX = 50.0f;
	float cicleSize = 2335;
	int i = 0;
	int j = 0;
	while (file >> y)
	{
		circlePoint.push_back(y);
		++i;
		if (i % 2335 == 0)
		{
			surveyTread.insert(startX + 0.1f*j, circlePoint);
			circlePoint.clear();
			++j;
		}
	}
	file.close();

	QVector<QVector2D> surveyFlange;
	file.open(FilePath::kSurveyFlangePath);
	if (!file.is_open()) {
		QString error = "ERROR::QLocalOpenGLWidget::ReadTxtMsg::UNABLE TO OPEN FILE Flange";
		QMessageBox::warning(NULL, "ERROR!", error, QMessageBox::Ok);
	}
	float x;
	while (file >> x >> y)
	{
		surveyFlange.push_back(QVector2D(x, y));
	}
	file.close();

	//this->AddWheel(surveyFlange, surveyTread, "SH840-4L");

	Wheel::WheelFlangeTread wheel_flange_tread(surveyFlange, surveyTread);//可以用来生成标准轮数据

	//this->AddWheelSpread(wheel_flange_tread.LoadSpreadPoint());
	this->AddWheelFlangeTread(wheel_flange_tread);
}
void QLocalSpreadOpenglWidget::AddWheelFlangeTread(const Wheel::WheelFlangeTread& wft_msg)
{
	this->wheel_flange_treads.push_back(wft_msg);
	this->AddWheelSpread(wft_msg.LoadSpreadFlangeTreadPoint(/*this->standard_wheel_makers[FilePath::kExampleStandardTreadPath])*/));//对比的标准轮为默认数据
	if (this->currentWheelId == -1)//默认currentWheelID
	{
		this->SetCurrentWheel(0);
	}
}
void QLocalSpreadOpenglWidget::AddWheelSpread(const Wheel::WheelSpreadMsg& spmsg)
{
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
	//x方向进行放大，放大到指定长度
	float factor = kSpreadExtendLength / Wheel::WheelFlangeTread::kSpreadCircumferenceLength;
	model = glm::scale(model, glm::vec3(factor, 1.0f, 1.0f));

	glm::vec3 color(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);

	this->makeCurrent();
	this->engine.AddWheel(OpenglObj::Mesh(spmsg.wheelMsg.GetVertices(), spmsg.wheelMsg.GetIndices()), model, color);
	this->doneCurrent();

	this->wheelSpreadParm.push_back(spmsg.wheelSpreadParm);
	this->wheelSpreadParm[this->wheelSpreadParm.size() - 1].set_color_gamut(kDefaultColorGamut);//设定默认色域

	if (this->currentWheelId == -1)//默认currentWheelID
	{
		this->SetCurrentWheel(0);
	}
}

void QLocalSpreadOpenglWidget::SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread)
{
	/*
	if (wheel_ids.find(wheel_id.toStdString()) != wheel_ids.end())
	{
		this->SetCurrentWheel(wheel_ids[wheel_id.toStdString()]);
		return;
	}

	Wheel::WheelFlangeTread wheel(surveyFlange, surveyTread);

	int id;

	id = this->wheel_flange_treads.size();
	this->wheel_flange_treads.push_back(wheel);

	Wheel::WheelSpreadMsg spmsg = wheel.LoadSpreadFlangeTreadPoint();

	this->wheelSpreadParm.push_back(spmsg.wheelSpreadParm);
	this->wheelSpreadParm[this->wheelSpreadParm.size() - 1].set_color_gamut(kDefaultColorGamut);//设定默认色域

	wheel_ids.insert({ wheel_id.toStdString(),id });

	this->AddOpenglMsg(spmsg);
	*/

	//std::cout << "QLocalSpreadOpenglWidget::SetWheel::1" << std::endl;
	if (!this->thread)
	{
		this->CreateThread();
	}
	//std::cout << "QLocalSpreadOpenglWidget::SetWheel::2" << std::endl;
	emit StartAddWheelThread(wheel_id, surveyFlange, surveyTread,  &this->wheel_ids, &this->wheel_flange_treads, &this->wheelSpreadParm);//主线程通过信号换起子线程的槽函数
}


void QLocalSpreadOpenglWidget::CreateThread()
{
	if (this->thread)
	{
		return;
	}
	//std::cout << "QLocalSpreadOpenglWidget::CreateThread::1" << std::endl;
	//std::cout << "QLocalSpreadOpenglWidget::CreateThread::is_context_init::" << this->is_context_init << std::endl;
	this->thread = new QThread();
	this->worker = new QLocalSpreadOpenGLWidgetThread(this->is_context_init);
	this->worker->moveToThread(this->thread);

	connect(this->thread, &QThread::finished, this->thread, &QObject::deleteLater);
	connect(this->thread, &QThread::finished, this->worker, &QObject::deleteLater);

	connect(this, &QLocalSpreadOpenglWidget::StartAddWheelThread, this->worker, &QLocalSpreadOpenGLWidgetThread::Run);
	//connect(this, &QLocalOpenGLWidget::startObjThreadWork2, m_obj, &ThreadObject::runSomeBigWork2);
	connect(this->worker, &QLocalSpreadOpenGLWidgetThread::ProgressLog, this, &QLocalSpreadOpenglWidget::ProcessLog);
	connect(this->worker, &QLocalSpreadOpenGLWidgetThread::ErrorLog, this, &QLocalSpreadOpenglWidget::ErrorLog);
	connect(this->worker, static_cast<void (QLocalSpreadOpenGLWidgetThread::*)(int,const Wheel::WheelSpreadMsg&)>(&QLocalSpreadOpenGLWidgetThread::Result), this, &QLocalSpreadOpenglWidget::AddOpenglMsg);
	connect(this->worker, static_cast<void(QLocalSpreadOpenGLWidgetThread::*)(int)>(&QLocalSpreadOpenGLWidgetThread::Result), this, &QLocalSpreadOpenglWidget::SetCurrentWheel);
	this->thread->start();
}
void QLocalSpreadOpenglWidget::ContextIsInit()
{
	//std::cout << "QLocalSpreadOpenglWidget::ContextIsInit" << std::endl;
	this->is_context_init = true;
	
	if (this->worker!=nullptr)
	{
		//std::cout << "QLocalSpreadOpenglWidget::ContextIsInit::IN?" << std::endl;
		this->worker->set_context_init_state(true);
	}

	
	//if (this->context()->isValid())
	//{
	//	std::cout << "IS Valid" << std::endl;
	//}
	//else
	//{
	//	std::cout << "IS  Not Valid" << std::endl;
	//}

	//if (this->context()->isOpenGLES())
	//{
	//	std::cout << "IS OPENGL ES" << std::endl;
	//}
	//else
	//{
	//	std::cout << "IS  Not OPENGL ES" << std::endl;
	//}
}
void QLocalSpreadOpenglWidget::AddOpenglMsg(int wheel_id,const Wheel::WheelSpreadMsg& spmsg)
{
	//std::cout << "QLocalSpreadOpenglWidget::AddOpenglMsg::Start" << std::endl;
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
															  //x方向进行放大，放大到指定长度
	float factor = kSpreadExtendLength / Wheel::WheelFlangeTread::kSpreadCircumferenceLength;
	model = glm::scale(model, glm::vec3(factor, 1.0f, 1.0f));

	glm::vec3 color(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);

	this->makeCurrent();
	this->engine.AddWheel(OpenglObj::Mesh(spmsg.wheelMsg.GetVertices(), spmsg.wheelMsg.GetIndices()), model, color);
	this->doneCurrent();

	//std::cout << "QLocalSpreadOpenglWidget::AddOpenglMsg" << std::endl;
	if (wheel_id !=0)//默认currentWheelID
	{
		std::cout << "Error::QLocalSpreadOpenglWidget::AddOpenglMsg::illegal wheel id" << std::endl;
		return;
	}
	else
	{
		this->SetCurrentWheel(wheel_id);
	}
	//std::cout << "QLocalSpreadOpenglWidget::AddOpenglMsg::End" << std::endl;
}
void QLocalSpreadOpenglWidget::AddTestMsg()
{
	auto size=this->wheel_flange_treads.size();
	if (size == 0)
	{
		std::cout << "Error::QLocalSpreadOpenglWidget::AddTestMsg::illegal size" << std::endl;
	}
	int id = size;
	this->wheel_ids.insert({ "Test",id });
	this->wheel_flange_treads.push_back(this->wheel_flange_treads[0]);
	this->wheelSpreadParm.push_back(this->wheelSpreadParm[0]);

	auto spmsg=this->wheel_flange_treads[0].LoadSpreadFlangeTreadPoint();

	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
															  //x方向进行放大，放大到指定长度
	float factor = kSpreadExtendLength / Wheel::WheelFlangeTread::kSpreadCircumferenceLength;
	model = glm::scale(model, glm::vec3(factor, 1.0f, 1.0f));

	glm::vec3 color(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);

	this->makeCurrent();
	auto mesh = OpenglObj::Mesh(spmsg.wheelMsg.GetVertices(), spmsg.wheelMsg.GetIndices());

	//this->engine.AddWheel(mesh, model, color);
	this->doneCurrent();

	this->update();
	////std::cout << "QLocalSpreadOpenglWidget::AddOpenglMsg" << std::endl;
	//if (id < 0 || id >= this->wheel_flange_treads.size())//默认currentWheelID
	//{
	//	std::cout << "Error::QLocalSpreadOpenglWidget::AddOpenglMsg::illegal wheel id" << std::endl;
	//	return;
	//}
	//else
	//{
	//	//if (id < 0 || id >= this->engine.GetWheelsSize())
	//	//{
	//	//	std::cout << "ERROR::QLocalSpreadOpenglWidget::SETCURRENTWHEEL::ILLEGAL ID!" << std::endl;
	//	//}
	//	//else
	//	//{
	//	//	this->currentWheelId = id;

	//	//	pickMsg.SetLength(0.0f);
	//	//	pickMsg.SetWidth(70.0f);

	//	//	//切换轮子时重置shader
	//	//	this->makeCurrent();
	//	//	wheel_spread_shader_program.use();
	//	//	wheel_spread_shader_program.setFloat("pickLength", pickMsg.GetLength());
	//	//	wheel_spread_shader_program.setFloat("pickWidth", pickMsg.GetWidth());
	//	//	wheel_spread_shader_program.setFloat("unitLength", this->wheelSpreadParm[currentWheelId].get_unit_length());
	//	//	wheel_spread_shader_program.setFloat("unitWidth", this->wheelSpreadParm[currentWheelId].get_unit_width());

	//	//	//wheel_spread_shader_program.setFloat("originHeight", 0.0f);
	//	//	wheel_spread_shader_program.setFloat("max_y", this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMax());
	//	//	wheel_spread_shader_program.setFloat("min_y", this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMin());
	//	//	wheel_spread_shader_program.setFloat("color_gamut", this->wheelSpreadParm[currentWheelId].get_color_gamut());
	//	//	wheel_spread_shader_program.setFloat("widthMax", 105.0f);

	//	//	this->doneCurrent();

	//	//	std::cout << "QLocalSpreadOpenglWidget::SetCurrentWheel::id:" << id << std::endl;
	//	//	this->UpdateStandardFlangeTreadAndColorModelForShader();
	//	//	//this->UpdateColorGamutForColorStripAndShader();

	//	//	this->InitCamera();

	//	//	//this->ReplaceStandardWheelMsg(DxfReader::StandardFlangeTreadMsgMaker());

	//	//	//this->engine.SetCurrentWheel(id);
	//	//}
	//}
}
//void QLocalSpreadOpenglWidget::ColorModel(unsigned char color_model)
//{
//	if (color_model != Wheel::kHeightColorModel && color_model != Wheel::kContrastColorModel)
//	{
//		std::cout << "Error::QLocalSpreadOpenglWidget::ColorModel::illegal color_model" << std::endl;
//		return;
//	}
//	this->color_model = color_model;
//
//	this->makeCurrent();
//	wheel_spread_shader_program.use();
//	wheel_spread_shader_program.setInt("color_model", this->color_model);
//	this->doneCurrent();
//}

void QLocalSpreadOpenglWidget::ReplaceStandardWheelMsg(const DxfReader::StandardFlangeTreadMsgMaker& msg_maker)
{
	if (msg_maker.empty())
	{
		this->wheelSpreadParm[this->currentWheelId].set_standard_tread(std::vector<glm::vec2>());//传空
		this->UpdateStandardFlangeTreadAndColorModelForShader();//更新状态

		this->color_gradient_strip = CreateColorGradientStrip(
			kColorStripPosition_X,
			kColorStripPosition_Y,
			kColorStripWidth,
			kColorStripHeight,
			kColorStripTextSize,
			this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMin(),
			this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMax(),
			4);
		this->makeCurrent();
		this->color_gradient_strip.LoadOpenglMesh(this->size().width(), this->size().height(), kColorStripSize);
		this->doneCurrent();
		return;
	}

	//if (this->standard_wheel_makers.find(file_path) == this->standard_wheel_makers.end())
	//{
	//	Wheel::StandardFlangeTreadMsgMake msg_make(file_path);//数据读取错误会报错
	//	this->standard_wheel_makers.insert({ file_path,msg_make });
	//}

	if (this->currentWheelId == -1)
	{
		return;
	}
	else
	{
		auto st_msg=this->wheel_flange_treads[this->currentWheelId].LoadStandardTread(msg_maker);
		this->wheelSpreadParm[this->currentWheelId].set_standard_tread(st_msg);
		//this->SetCurrentWheel(this->currentWheelId);//更新当前状态,将新的标准点赋值给shader
		this->UpdateStandardFlangeTreadAndColorModelForShader();

		this->color_gradient_strip = CreateColorGradientStrip(
			kColorStripPosition_X,
			kColorStripPosition_Y,
			kColorStripWidth,
			kColorStripHeight,
			kColorStripTextSize,
			-this->wheelSpreadParm[currentWheelId].get_color_gamut(),
			this->wheelSpreadParm[currentWheelId].get_color_gamut(),
			4);
		this->makeCurrent();
		this->color_gradient_strip.LoadOpenglMesh(this->size().width(), this->size().height(), kColorStripSize);
		this->doneCurrent();
	}
}
void QLocalSpreadOpenglWidget::SetColorGamut(float color_gamut)
{
	if (currentWheelId == -1)
	{
		return;
	}

	int st_size = this->wheelSpreadParm[this->currentWheelId].get_standard_tread().size();

	if(st_size==0)
	{
		return;
	}
	else
	{
		if (color_gamut <= 0)
		{
			std::cout << "Error::QLocalSpreadOpenglWidget::SetColorGamut::illegal color gamut" << std::endl;
			return;
		}
		else
		{
			this->wheelSpreadParm[this->currentWheelId].set_color_gamut(color_gamut);
			this->UpdateColorGamutForColorStripAndShader();
		}
	}
}
void QLocalSpreadOpenglWidget::UpdateColorGamutForColorStripAndShader()
{
	//std::cout << "QLocalSpreadOpenglWidget::UpdateColorGamutForColorStripAndShader::1" << std::endl;
	int st_size = this->wheelSpreadParm[this->currentWheelId].get_standard_tread().size();
	if (st_size != 0)
	{
		//std::cout << "QLocalSpreadOpenglWidget::UpdateColorGamutForColorStripAndShader::size!=0" << std::endl;

		this->color_gradient_strip = CreateColorGradientStrip(  //刷新颜色表
			kColorStripPosition_X,
			kColorStripPosition_Y,
			kColorStripWidth,
			kColorStripHeight,
			kColorStripTextSize,
			-this->wheelSpreadParm[currentWheelId].get_color_gamut(),
			this->wheelSpreadParm[currentWheelId].get_color_gamut(),
			4);

		//刷新shader
		this->makeCurrent();
		this->color_gradient_strip.LoadOpenglMesh(this->size().width(), this->size().height(), kColorStripSize);

		wheel_spread_shader_program.use();
		wheel_spread_shader_program.setFloat("color_gamut", this->wheelSpreadParm[currentWheelId].get_color_gamut());
		this->doneCurrent();
	}
	else
	{
		//std::cout << "QLocalSpreadOpenglWidget::UpdateColorGamutForColorStripAndShader::size==0" << std::endl;

		this->color_gradient_strip = CreateColorGradientStrip(  //刷新颜色表
			kColorStripPosition_X,
			kColorStripPosition_Y,
			kColorStripWidth,
			kColorStripHeight,
			kColorStripTextSize,
			this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMin(),
			this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMax(),
			4);

		this->makeCurrent();
		this->color_gradient_strip.LoadOpenglMesh(this->size().width(), this->size().height(), kColorStripSize);
		this->doneCurrent();

		//this->color_gradient_strip.show();
		//std::cout << "Error::QLocalSpreadOpenglWidget::UpdateColorGamut::update color gamut in height contrast" << std::endl;
	}
}

void QLocalSpreadOpenglWidget::UpdateStandardFlangeTreadAndColorModelForShader()
{
	int size = this->wheelSpreadParm[currentWheelId].get_standard_tread().size();

	this->makeCurrent();
	wheel_spread_shader_program.use();
	if (size == 0)
	{
		wheel_spread_shader_program.setInt("color_model",0);
	}
	else
	{
		wheel_spread_shader_program.setInt("color_model", 1);
		for (int i = 0; i != size; ++i)
		{
			std::string name = "standardTreadMsg[" + std::to_string(i) + "]";
			wheel_spread_shader_program.setVec2(name, this->wheelSpreadParm[currentWheelId].get_standard_tread()[i]);
		}
		wheel_spread_shader_program.setInt("standardTreadMsgSize", size);
	}
	this->doneCurrent();
}

//void QLocalSpreadOpenglWidget::AddDefaultStandardWheelMaker()
//{//相对路径和绝对路径？
//	if (this->standard_wheel_makers.find(FilePath::kExampleStandardTreadPath) != this->standard_wheel_makers.end())
//	{
//		std::cout << "Error::QLocalSpreadOpenglWidget::AddDefaultStandardWheelMaker::this path is already exist" << std::endl;
//		return;
//	}
//	Wheel::StandardFlangeTreadMsgMake msg_make(FilePath::kExampleStandardTreadPath);
//	this->standard_wheel_makers.insert({ FilePath::kExampleStandardTreadPath,msg_make });
//}
void QLocalSpreadOpenglWidget::InitCartesianCoordinate()
{
	this->makeCurrent();
	float coordinate_text_size = 8;
	Coordinate::CartesianCoordinate coordinate;
	float x_length = 150.0f;
	

	coordinate.set_axis_length(150.0f, 700.0f, 100.0f);
	coordinate.set_axis_extend(30.0f);
	coordinate.set_arrow_width(5.0f);
	/*coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(70.0f / 135.0f, Text("70", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(135.0f / 135.0f, Text("135", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(35.0f / 135.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(105.0f / 135.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));*/

	float start_mark = 50.0f;
	std::string unit = "mm";
	for (int i = 0; i != 3; ++i)
	{
		std::string text;
		std::stringstream ss;
		ss << start_mark;
		ss >> text;//或者 res = ss.str();
		coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(start_mark / 150.0f, Text(text+ unit, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
		start_mark += 50.0f;
	}

	start_mark = 25.0f;
	for (int i = 0; i != 3; ++i)
	{
		coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(start_mark / 150.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
		start_mark += 50.0f;
	}


	//start_mark = 100.0f;
	//for (int i = 0; i != 7; ++i)
	//{
	//	std::string text;
	//	std::stringstream ss;
	//	ss << start_mark;
	//	ss >> text;//或者 res = ss.str();
	//	coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(start_mark / 700.0f, Text(text+ unit, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	//	start_mark += 100.0f;
	//}

	//coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(0.0f / 700.0f, Text("(0d)", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(100.0f / 700.0f, Text("100mm\n(60d)", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(200.0f / 700.0f, Text("200mm\n(120d)", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(300.0f / 700.0f, Text("300mm\n(180d)", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(400.0f / 700.0f, Text("400mm\n(240d)", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(500.0f / 700.0f, Text("500mm\n(300d)", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(600.0f / 700.0f, Text("600mm\n(360d)", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(700.0f / 700.0f, Text("700mm", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	start_mark = 50.0f;
	for (int i = 0; i != 7; ++i)
	{
		coordinate.AddMark(Coordinate::Axis::Y, Coordinate::Mark(start_mark / 700.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
		start_mark += 100.0f;
	}

	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(0.0f / 100.0f, Text("-50" + unit, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(25.0f / 100.0f, Text("-25" + unit, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(50.0f / 100.0f, Text("0" + unit, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(75.0f / 100.0f, Text("25" + unit, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::Z, Coordinate::Mark(100.0f / 100.0f, Text("50" + unit, coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));

	glm::mat4 coordinate_model(1.0f);
	coordinate_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*coordinate_model;
	coordinate_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))*coordinate_model;
	coordinate_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -50.0f, 0.0f))*coordinate_model;
	coordinate_model = scale*coordinate_model;

	//
	//<< "h_ruler!" << std::endl;
	auto coordinate_object = coordinate.LoadObject(this->font_texture);
	coordinate_object.SetModel(coordinate_model);
	
	engine.AddCoordinate(coordinate_object);

	this->doneCurrent();
}
void QLocalSpreadOpenglWidget::InitRuler()
{
	this->makeCurrent();//注意
	float ruler_text_size = kHRulerMarkTextDisToMark;
	float h_ruler_length = Wheel::WheelFlangeTread::kStandardFlangeTreadWidth;
	Coordinate::Ruler h_ruler(h_ruler_length);//竖直
	h_ruler.set_mark_text_dis_to_mark(kHRulerMarkTextDisToMark);
	h_ruler.add_mark(Coordinate::Mark(0.0f / h_ruler_length, Text("0(mm)", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	h_ruler.add_mark(Coordinate::Mark(70.0f / h_ruler_length,Text("70", ruler_text_size,glm::vec3(1.0f,1.0f,1.0f))));
	h_ruler.add_mark(Coordinate::Mark(135.0f / h_ruler_length, Text("135", ruler_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	glm::mat4 scale= glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));

	glm::mat4 h_ruler_model(1.0f);
	h_ruler_model = scale*h_ruler_model;
	h_ruler_model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))*h_ruler_model;
	h_ruler_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f))*h_ruler_model;
	h_ruler_model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.39f, 0.0f, 0.0f))*h_ruler_model;

	//std::cout << "h_ruler!" << std::endl;
	auto h_ruler_object = h_ruler.LoadObject(this->font_texture);
	h_ruler_object.SetModel(h_ruler_model);

	Coordinate::Ruler v_ruler(kSpreadExtendLength);
	glm::vec3 mark_text_color = glm::vec3(0.0f, 1.0f, 1.0f);
	v_ruler.set_color(mark_text_color);

	v_ruler.set_mark_text_dis_to_mark(10.0f);
	v_ruler.add_mark(Coordinate::Mark(0.0f / 360.0f, mark_text_color, Text("0(d)", ruler_text_size, mark_text_color)));
	v_ruler.add_mark(Coordinate::Mark(45.0f / 360.0f, mark_text_color, Text("45", ruler_text_size, mark_text_color)));
	v_ruler.add_mark(Coordinate::Mark(90.0f / 360.0f, mark_text_color, Text("90", ruler_text_size, mark_text_color)));
	v_ruler.add_mark(Coordinate::Mark(135.0f / 360.0f, mark_text_color, Text("135", ruler_text_size, mark_text_color)));
	v_ruler.add_mark(Coordinate::Mark(180.0f / 360.0f, mark_text_color, Text("180", ruler_text_size, mark_text_color)));
	v_ruler.add_mark(Coordinate::Mark(225.0f / 360.0f, mark_text_color, Text("225", ruler_text_size, mark_text_color)));
	v_ruler.add_mark(Coordinate::Mark(270.0f / 360.0f, mark_text_color, Text("270", ruler_text_size, mark_text_color)));
	v_ruler.add_mark(Coordinate::Mark(315.0f / 360.0f, mark_text_color, Text("315", ruler_text_size, mark_text_color)));
	v_ruler.add_mark(Coordinate::Mark(360.0f / 360.0f, mark_text_color, Text("360", ruler_text_size, mark_text_color)));

	glm::mat4 v_ruler_model(1.0f);
	v_ruler_model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))*v_ruler_model;
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f))*model;
	v_ruler_model = scale*v_ruler_model;
	v_ruler_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.05f, 1.4f))*v_ruler_model;

	//std::cout << "v_ruler!" << std::endl;
	auto v_ruler_object = v_ruler.LoadObject(this->font_texture);
	v_ruler_object.SetModel(v_ruler_model);

	engine.AddRuler(h_ruler_object);
	engine.AddRuler(v_ruler_object);

	this->doneCurrent();
}
void QLocalSpreadOpenglWidget::InitColorGradientStrip()
{
	if (currentWheelId == -1)
	{
		this->color_gradient_strip=CreateColorGradientStrip(
			kColorStripPosition_X, 
			kColorStripPosition_Y,
			kColorStripWidth, 
			kColorStripHeight, 
			kColorStripTextSize, 
			0.0f, 0.0f, 0);
		this->makeCurrent();
		this->color_gradient_strip.LoadOpenglMesh(this->size().width(), this->size().height(), kColorStripSize);
		this->doneCurrent();
	}
	else
	{
		std::cout << "Warning::QLocalSpreadOpenglWidget::InitColorGradientStrip." << std::endl;
		this->color_gradient_strip=CreateColorGradientStrip(
			kColorStripPosition_X, 
			kColorStripPosition_Y, 
			kColorStripWidth, 
			kColorStripHeight, 
			kColorStripTextSize, 
			this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMin(), 
			this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMax(), 
			4);
		this->makeCurrent();
		this->color_gradient_strip.LoadOpenglMesh(this->size().width(), this->size().height(), kColorStripSize);
		this->doneCurrent();
	}
}
void QLocalSpreadOpenglWidget::SetCurrentWheel(int id)
{
	if (id < 0 || id >= this->engine.GetWheelsSize())
	{
		std::cout << "ERROR::QLocalSpreadOpenglWidget::SETCURRENTWHEEL::ILLEGAL ID!" << std::endl;
	}
	else
	{
		this->currentWheelId = id;

		pickMsg.SetLength(0.0f);
		pickMsg.SetWidth(70.0f);
		
		emit PickSectionAngle(0.0f);
		emit PickSectionRadian(0.0f);//发radian信号

		//切换轮子时重置shader
		this->makeCurrent();
		wheel_spread_shader_program.use();
		wheel_spread_shader_program.setFloat("pickLength", pickMsg.GetLength());
		wheel_spread_shader_program.setFloat("pickWidth", pickMsg.GetWidth());
		wheel_spread_shader_program.setFloat("unitLength", this->wheelSpreadParm[currentWheelId].get_unit_length());
		wheel_spread_shader_program.setFloat("unitWidth", this->wheelSpreadParm[currentWheelId].get_unit_width());

		//wheel_spread_shader_program.setFloat("originHeight", 0.0f);
		wheel_spread_shader_program.setFloat("max_y", this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMax());
		wheel_spread_shader_program.setFloat("min_y", this->wheelSpreadParm[currentWheelId].get_spread_y_range().GetMin());
		wheel_spread_shader_program.setFloat("color_gamut", this->wheelSpreadParm[currentWheelId].get_color_gamut());
		wheel_spread_shader_program.setFloat("widthMax", 105.0f);

		this->doneCurrent();

		//std::cout << "QLocalSpreadOpenglWidget::SetCurrentWheel::id:"<<id << std::endl;
		this->UpdateStandardFlangeTreadAndColorModelForShader();
		this->UpdateColorGamutForColorStripAndShader();

		this->InitCamera();

		//this->ReplaceStandardWheelMsg(DxfReader::StandardFlangeTreadMsgMaker());

		//this->engine.SetCurrentWheel(id);
	}
}


void QLocalSpreadOpenglWidget::resizeGL(int w, int h)
{
	this->scr_width = w;
	this->scr_height = h;
	glViewport(0, 0, w, h);

	this->makeCurrent();
	this->color_gradient_strip.LoadOpenglMesh(this->size().width(), this->size().height(), kColorStripSize);
	this->doneCurrent();

}
//void QLocalSpreadOpenglWidget::showEvent(QShowEvent *e)
//{
//	QOpenGLWidget::showEvent(e);
//	//this->InitPickValue();
//}
void QLocalSpreadOpenglWidget::InitShaderValue()
{
	this->makeCurrent();
	wheel_spread_shader_program.use();
	//shaderProgram.setFloat("pickLength", pickMsg.GetLength());
	//shaderProgram.setFloat("pickWidth", pickMsg.GetWidth());
	this->doneCurrent();
}
void QLocalSpreadOpenglWidget::paintGL()
{
	this->DrawOpengl();

	this->Draw2DText();

}
void QLocalSpreadOpenglWidget::Draw2DText(/*QPainter& painter*/)  //only use for paintGL
{
	if(this->currentWheelId != -1)
		this->color_gradient_strip.Draw(_2d_shader_program);
}
void QLocalSpreadOpenglWidget::DrawOpengl()//only use for paintGL
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//设定清屏所用的颜色
										 //glClear(GL_COLOR_BUFFER_BIT);//接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wheel_spread_shader_program.use();
	wheel_spread_shader_program.setVec3("objectColor", 192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
	wheel_spread_shader_program.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	wheel_spread_shader_program.setVec3("lightPos", camera.position);
	wheel_spread_shader_program.setVec3("viewPos", camera.position);

	//view/projection
	view = camera.GetViewMatrix();//3d摄像机
	wheel_spread_shader_program.setMat4("view", view);
	//glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(camera.zoom), ((float)this->scr_width / this->scr_height), 0.1f, 100.0f);
	wheel_spread_shader_program.setMat4("projection", projection);

	if (this->currentWheelId == 0)
	{
		engine.DrawWheel(wheel_spread_shader_program, this->currentWheelId);
	}


	line_shader_program.use();
	line_shader_program.setMat4("view", view);
	line_shader_program.setMat4("projection", projection);

	//engine.DrawLines(line_shader_program);
	//this->coordinate_line.Draw(line_shader_program);
	font_shader_program.use();
	font_shader_program.setMat4("view", view);
	font_shader_program.setMat4("projection", projection);

	engine.UpdateTextDirection(camera.front, camera.up, projection, view);
	//engine.DrawRulers(line_shader_program,font_shader_program);
	
	if (this->ruler_visible && this->currentWheelId == 0)
	{
		engine.DrawRulers(line_shader_program, font_shader_program);
		//std::cout << "?" << std::endl;
		//engine.DrawRulers(line_shader_program, font_shader_program);
	}

	if (this->coordinate_visible&& this->currentWheelId == 0)
	{
		engine.DrawCoordinates(line_shader_program, font_shader_program);
	}

	glDisable(GL_DEPTH_TEST);
}
void QLocalSpreadOpenglWidget::paintEvent(QPaintEvent *e)
{
	//QPainter painter(this);
	//painter.drawText(0, 0, "Hellow Opengl!");
	//this->makeCurrent();
	//this->paintGL();
	//QPainter pter(this);
	//pter.setPen(Qt::blue);
	//pter.drawText(20, 50, "This is a Text!");
	//pter.end();
	//update();



	QOpenGLWidget::paintEvent(e);
}
void QLocalSpreadOpenglWidget::SetPickRadian(double radian)
{
	if (currentWheelId != -1)
	{
		if (radian >= 0 && radian <= 2 * Wheel::WheelFlangeTread::kPIR)
		{
			Range lengthRange = wheelSpreadParm[currentWheelId].get_length_range();
			float pickLength =/* lengthRange.GetStart() - */lengthRange.GetDifference()*radian / (2 * Wheel::WheelFlangeTread::kPIR);
			
			if (abs(pickLength - this->pickMsg.GetLength()) > wheelSpreadParm[currentWheelId].get_unit_length())
			{
				this->pickMsg.SetLength(pickLength);
				this->makeCurrent();
				wheel_spread_shader_program.use();
				wheel_spread_shader_program.setFloat("pickLength", pickLength);  //是-1可以刷新数据
				this->doneCurrent();
			}
		}
		else
		{
			std::cout << "ERROR::QLocalSpreadOpenglWidget::SetPickRadian::ILLEGAL RADIAN" << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR::QLocalSpreadOpenglWidget::SetPickRadian::NO WHEEL TO SET PICK RADIAN!" << std::endl;
	}
}
void QLocalSpreadOpenglWidget::SetPickAngle(double angle)
{
	if (currentWheelId != -1)
	{
		if (angle >= 0 && angle <= 360)
		{
			float radian = (2 * Wheel::WheelFlangeTread::kPIR)*angle / 360;
			Range lengthRange = wheelSpreadParm[currentWheelId].get_length_range();
			float pickLength = /*lengthRange.GetStart() - */lengthRange.GetDifference()*radian / (2 * Wheel::WheelFlangeTread::kPIR);
			if (abs(pickLength - this->pickMsg.GetLength()) > wheelSpreadParm[currentWheelId].get_unit_length())
			{
				this->pickMsg.SetLength(pickLength);
				this->makeCurrent();
				wheel_spread_shader_program.use();
				wheel_spread_shader_program.setFloat("pickLength", pickLength);  //是-1可以刷新数据
				this->doneCurrent();
			}
		}
		else
		{
			std::cout << "ERROR::QLocalSpreadOpenglWidget::SetPickAngle::ILLEGAL Angle" << std::endl;

		}
	}
	else
	{
		std::cout << "ERROR::QLocalSpreadOpenglWidget::SetPickAngle::NO WHEEL TO SET PICK Angle!" << std::endl;
	}
}
void QLocalSpreadOpenglWidget::mousePressEvent(QMouseEvent *e)
{
	this->setFocus();

	if (e->button() == Qt::LeftButton)
	{
		if (this->rotateAction)
		{
			setCursor(Qt::ClosedHandCursor);
			this->lastX = e->x();
			this->lastY = e->y();
		}
		else
		{
			this->Pick(e->x(), e->y());
		}
		this->leftButtonDown = true;
	}
	if (e->button() == Qt::RightButton)////右键取消
	{
		if (this->rotateAction)
		{
			this->rotateAction = false;
			emit RotateSignal(this->rotateAction);
			setCursor(Qt::ArrowCursor);
		}

		this->lastX = e->x();
		this->lastY = e->y();
		this->rightButtonDown = true;
	}
	if (e->button() == Qt::MiddleButton)
	{
		this->lastX = e->x();
		this->lastY = e->y();
		this->middleButtonDown = true;
	}

}
void QLocalSpreadOpenglWidget::mouseReleaseEvent(QMouseEvent *e)  //在非opengl函数中调用opengAPI需要makeCurrent
{
	if (e->button() == Qt::LeftButton)
	{
		if (this->rotateAction)
		{
			setCursor(Qt::OpenHandCursor);
		}

		this->leftButtonDown = false;
		//firstMouse = true;
	}
	if (e->button() == Qt::RightButton)
	{
		this->rightButtonDown = false;
	}

	if (e->button() == Qt::MiddleButton)
	{
		this->middleButtonDown = false;
	}
	//this->paintGL();
}



void QLocalSpreadOpenglWidget::mouseMoveEvent(QMouseEvent * e)
{
	if (rightButtonDown)
	{
		float xoffset = e->x() - this->lastX;
		float yoffset = this->lastY - e->y();

		//std::cout << "xoffset:" << xoffset << " yoffset:" << yoffset << endl;

		this->lastX = e->x();
		this->lastY = e->y();

		if (this->camera_move_enable)
		{
			camera.ProcessMouseMovement(xoffset, yoffset);

			float dis = sqrt(pow(xoffset, 2) + pow(yoffset, 2));
			if (dis <= kRoamingThreshold)
			{
				this->camera_roaming_enable = false;
			}
			else
			{
				this->camera_roaming_speed_x = kRoamingOffsetSpeedRatio*xoffset;
				this->camera_roaming_speed_y = kRoamingOffsetSpeedRatio*yoffset;
				this->camera_roaming_enable = true;
			}
		}
	}
	if (leftButtonDown)
	{
		if (rotateAction)
		{
			float xoffset = e->x() - this->lastX;
			float yoffset = this->lastY - e->y();
			this->lastX = e->x();
			this->lastY = e->y();

			if (this->camera_move_enable)
			camera.ProcessMouseMovement(xoffset, yoffset);
		}
		else
		{
			this->Pick(e->x(), e->y());

		}
	}
	if (middleButtonDown)
	{
		float xoffset = e->x() - this->lastX;
		float yoffset = this->lastY - e->y();
		this->lastX = e->x();
		this->lastY = e->y();

		if (this->camera_move_enable)
		camera.ProcessMiddleMouseMovement(xoffset, yoffset);
	}
}

void QLocalSpreadOpenglWidget::Pick(int posX, int posY)
{
	if (this->currentWheelId != -1)
	{
		//std::cout << "Pick 1" << std::endl;
		PickMsg locMsg = engine.RayPick_WheelSpread
		(
			posX, posY,
			projection, view,
			camera.position,
			this->scr_width, this->scr_height,
			Range(0, Wheel::WheelFlangeTread::kStandardFlangeTreadWidth),
			this->wheelSpreadParm[currentWheelId].get_length_range()
		);
		//std::cout <<"Length: "<< msg.GetLength() <<" Width: "<< msg.GetWidth() << std::endl;

		if (this->pickSectionEnable&&locMsg.GetLength() != -1)//防止空
		{
		
			this->makeCurrent();
			wheel_spread_shader_program.use();
			wheel_spread_shader_program.setFloat("pickLength", locMsg.GetLength());  //是-1可以刷新数据
			this->doneCurrent();
			//std::cout << "pickLength:" << locMsg.GetLength() << std::endl;

			//this->pickMsg.SetLength(this->wheelSpreadParm[currentWheelId].GetLengthRange().GetStart()- locMsg.GetLength());//真正的length
			this->pickMsg.SetLength(locMsg.GetLength());

			float radian=2* Wheel::WheelFlangeTread::kPIR*pickMsg.GetLength() / this->wheelSpreadParm[currentWheelId].get_length_range().GetDifference();
			//float angle = 360.0f*radian / (2 * WheelDisplay::Wheel::PIR);
			emit PickSectionRadian(radian);  //只有pick执行时才会发送改变gotoWidget的信号，set的时候不发
			emit PickSectionAngle(360.0f*radian / (2 * Wheel::WheelFlangeTread::kPIR));
		}

		//if (this->pickWidthEnable&&locMsg.GetWidth() != -1)
		//{
		//	Range range = wheelSpreadParm[currentWheelId].GetWidthRange();
		//	if (locMsg.GetWidth() >= range.GetMin() && locMsg.GetWidth() <= range.GetMax())  //再加以限制
		//	{
		//		this->pickMsg.SetWidth(locMsg.GetWidth());
		//		this->makeCurrent();
		//		shaderProgram.setFloat("pickWidth", locMsg.GetWidth());
		//		this->doneCurrent();
		//		std::cout << "pickWidth:" << locMsg.GetWidth() << std::endl;

		//		emit PickWidth(this->pickMsg.GetWidth());
		//	}
		//}
		//std::cout << "Pick 2" << std::endl;
	}
}
//void QLocalSpreadOpenglWidget::SetPickWidth(double width)
//{
//	if (currentWheelId != -1)
//	{
		//Range range = wheelSpreadParm[currentWheelId].GetWidthRange();
		//if (width >= range.GetMax() && width <= range.GetMin())
		//{
		//	if (abs(this->pickMsg.GetWidth() - width)>0.001)//查重
		//	{
		//		this->pickMsg.SetWidth(width);
		//		this->makeCurrent();
		//		shaderProgram.setFloat("pickWidth", width);
		//		this->doneCurrent();
		//		//std::cout << "pickWidth:" << width << std::endl;
		//		emit PickWidth(width);//也要发！
		//	}
		//}
		//else
		//{
		//	std::cout << "ERROR::QLocalSpreadOpenglWidget::SETPICKWIDTH::ILLEGAL WIDTH! " << range.GetStart() << " " << range.GetEnd() << std::endl;
		//}
//	}
//	else
//	{
//		std::cout << "ERROR::QLocalSpreadOpenglWidget::SETPICKWIDTH::NO WHEEL TO SET PICK WIDTH!" << std::endl;
//	}
//}

void QLocalSpreadOpenglWidget::timerEvent(QTimerEvent *e)
{
	//if (this->keyWPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::FORWARD, 0.001*TIMESPAN);
	//if(this->keySPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.001*TIMESPAN);
	//if(this->keyAPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::LEFT, 0.001*TIMESPAN);
	//if (this->keyDPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::RIGHT, 0.001*TIMESPAN);
	if (this->camera_roaming_enable)
	{
		this->camera.ProcessMouseMovement(this->camera_roaming_speed_x, this->camera_roaming_speed_y);
	}
	this->update();
}

void QLocalSpreadOpenglWidget::keyPressEvent(QKeyEvent * e)
{
	if (e->key() == Qt::Key_W)
		this->keyWPressEvent = true;
	if (e->key() == Qt::Key_S)
		this->keySPressEvent = true;
	if (e->key() == Qt::Key_A)
		this->keyAPressEvent = true;
	if (e->key() == Qt::Key_D)
		this->keyDPressEvent = true;
}
void QLocalSpreadOpenglWidget::keyReleaseEvent(QKeyEvent * e)
{
	if (e->key() == Qt::Key_W)
		this->keyWPressEvent = false;
	if (e->key() == Qt::Key_S)
		this->keySPressEvent = false;
	if (e->key() == Qt::Key_A)
		this->keyAPressEvent = false;
	if (e->key() == Qt::Key_D)
		this->keyDPressEvent = false;
}

void QLocalSpreadOpenglWidget::wheelEvent(QWheelEvent * e)
{
	if (this->camera_move_enable)
	camera.ProcessMouseScroll(e->delta());
	//this->paintGL();
}

void QLocalSpreadOpenglWidget::Bigger()
{
	if (this->camera_move_enable)
	camera.ProcessMouseScroll(120);
	//if (!camera.ProcessMouseScroll(120))
	//{
	//	this->bigEnable = false;
	//	emit BiggerSignal(this->bigEnable);
	//}
	//else
	//{
	//	if (!this->smallerEnable)
	//	{
	//		this->smallerEnable = true;
	//		emit SmallerSignal(this->smallerEnable);
	//	}
	//}
}

void QLocalSpreadOpenglWidget::Smaller()
{
	if (this->camera_move_enable)
	camera.ProcessMouseScroll(-120);
	/*if (!camera.ProcessMouseScroll(-120))
	{
	this->smallerEnable = false;
	emit SmallerSignal(this->smallerEnable);
	}
	else
	{
	if (!this->bigEnable)
	{
	this->bigEnable = true;
	emit BiggerSignal(this->bigEnable);
	}
	}*/
}

void QLocalSpreadOpenglWidget::InitCamera()
{
	//camera.InitLocalSpreadCamera();
	if (this->full_screen_enable == false)
	{
		this->camera_move_enable = false;

		glm::vec3 focus;
		if (this->currentWheelId != -1)
		{
			//focus.x = (wheelSpreadParm[this->currentWheelId].get_length_range().GetMin() + wheelSpreadParm[this->currentWheelId].get_length_range().GetMax()) / 2.0f - 10;
			focus.x = kSpreadExtendLength / 2.0f + kCameraFocusOffset;
			focus.z = (0.0f + Wheel::WheelFlangeTread::kStandardFlangeTreadWidth) / 2.0f + 10;
			focus.y = (wheelSpreadParm[this->currentWheelId].get_spread_y_range().GetMin() + wheelSpreadParm[this->currentWheelId].get_spread_y_range().GetMax()) / 2.0f/* - Wheel::WheelFlangeTread::kStandardTreadRadius*/;

			glm::mat4 model(1.0f);
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
			focus = model*glm::vec4(focus.x, focus.y, focus.z, 1.0f);
		}
		else
		{
			focus = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		camera.InitCamera(kYaw, kPitch, kCameraLocalAngle, focus, kNoFullScreenR, kZoom);
		camera.SetMouseSensitivity(kMouseSensitivity);
		camera.SetMiddleMouseSensitivity(kMiddleMouseSensitivity);
		camera.SetWheelSensitivity(kWheelSensitivity);
	}
	else
	{
		this->camera_move_enable = true;

		glm::vec3 focus;
		if (this->currentWheelId != -1)
		{
			focus.x = (wheelSpreadParm[this->currentWheelId].get_length_range().GetMin() + wheelSpreadParm[this->currentWheelId].get_length_range().GetMax()) / 2.0f;
			//focus.x = kSpreadExtendLength / 2.0f;
			focus.z = (0.0f + Wheel::WheelFlangeTread::kStandardFlangeTreadWidth) / 2.0f + 10;
			focus.y = (wheelSpreadParm[this->currentWheelId].get_spread_y_range().GetMin() + wheelSpreadParm[this->currentWheelId].get_spread_y_range().GetMax()) / 2.0f/* - Wheel::WheelFlangeTread::kStandardTreadRadius*/;

			glm::mat4 model(1.0f);
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
			focus = model*glm::vec4(focus.x, focus.y, focus.z, 1.0f);
		}
		else
		{
			focus = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		camera.InitCamera(kYaw, kPitch, kCameraLocalAngle, focus, kFullScreenR, kZoom);
		camera.SetMouseSensitivity(kMouseSensitivity);
		camera.SetMiddleMouseSensitivity(kMiddleMouseSensitivity);
		camera.SetWheelSensitivity(kWheelSensitivity);
	}
}

void QLocalSpreadOpenglWidget::Rotate()
{
	//if (!this->rotateAction)
	//{
	//	this->rotateAction = true;
	//	setCursor(Qt::OpenHandCursor);
	//}
	//else  //再按一次关闭
	//{
	//	this->rotateAction = false;
	//	setCursor(Qt::ArrowCursor);
	//}

	if(this->camera_move_enable)
		this->camera.RotateX(-kRotateUnitOffset);
}



void QLocalSpreadOpenglWidget::EnableSectionPick()
{
	if (this->pickSectionEnable)
	{
		this->pickSectionEnable = false;
		this->makeCurrent();
		wheel_spread_shader_program.use();
		wheel_spread_shader_program.setFloat("pickRadian", -1);
		this->doneCurrent();
	}
	else
	{
		this->pickSectionEnable = true;
	}
}
void QLocalSpreadOpenglWidget::EnableWidthPick()
{
	if (this->pickWidthEnable)
	{
		this->pickWidthEnable = false;
		this->makeCurrent();
		wheel_spread_shader_program.use();
		wheel_spread_shader_program.setFloat("pickWidth", -1);
		this->doneCurrent();
	}
	else
	{
		this->pickWidthEnable = true;
	}

}
//    if(this->coordinateAxisVisible)
//    {
//        this->coordinateAxisVisible=false;
//    }
//    else
//    {
//        this->coordinateAxisVisible=true;
//    }

void QLocalSpreadOpenglWidget::ColorMarkVisible()
{

}

void QLocalSpreadOpenglWidget::CoordinateVisible()
{
	    if(this->coordinate_visible)
	    {
	        this->coordinate_visible =false;
	    }
	    else
	    {
	        this->coordinate_visible =true;
	    }
}
void QLocalSpreadOpenglWidget::RulerVisible()
{
	//std::cout << "?" << std::endl;
	    if(this->ruler_visible)
	    {
	        this->ruler_visible =false;
	    }
	    else
	    {
	        this->ruler_visible =true;
	    }
}

void QLocalSpreadOpenglWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (this->full_screen_enable == false)
		{
			emit FullScreen(true);
			this->full_screen_enable = true;
			this->UpdateWhenFullScreenOrRecover();
		}
		else
		{
			/*this->setWindowFlags(Qt::SubWindow);
			this->showNormal();*/
			emit FullScreen(false);
			this->full_screen_enable = false;
			this->UpdateWhenFullScreenOrRecover();
		}
	}
}

void QLocalSpreadOpenglWidget::UpdateWhenFullScreenOrRecover()
{
	if (this->full_screen_enable==true)
	{
		//标尺
		this->ruler_visible = false;
		this->coordinate_visible = true;

		//轮面
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放

		if (currentWheelId >= 0)
		{
			engine.SetWheelFlangeTreadModel(currentWheelId, model);
		}

		//camera
		this->InitCamera();
	}
	else
	{
		this->ruler_visible = true;
		this->coordinate_visible = false;

		//轮面
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放

		//x方向进行放大，放大到指定长度
		float factor = kSpreadExtendLength / Wheel::WheelFlangeTread::kSpreadCircumferenceLength;
		model = glm::scale(model, glm::vec3(factor, 1.0f, 1.0f));

		if (currentWheelId >=0)
		{
			engine.SetWheelFlangeTreadModel(currentWheelId, model);
		}

		//camera
		this->InitCamera();
	}
}