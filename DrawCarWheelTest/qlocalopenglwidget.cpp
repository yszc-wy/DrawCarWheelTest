#include "qlocalopenglwidget.hpp"

#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <QOpenGLTexture>
#include <qevent.h>
#include <glm/gtc/matrix_transform.hpp>
#include <qmessagebox.h>
#include <qdir.h>
#include <qopenglcontext.h>
#include <qsurface.h>

#include "LoadModel.h"
#include "Mesh.h"
#include "Vertex.h"
#include "CartesianCoordinate.h"
#include "PolarCoordinate.h"
#include "Mark.h"
#include "Text.h"
#include "DxfStandardFlangeTreadMsgMaker.h"
#include "DxfReader.h"
#include "CadLib\dl_dxf.h"
#include "CadLib\dl_creationadapter.h"
#include "DxfFloatPlantMsgMaker.h"
#include "qlocalopenglwidgetthread.h"

const float QLocalOpenGLWidget::kYaw = -160.0f;
const float QLocalOpenGLWidget::kPitch = 0.0f;
const float QLocalOpenGLWidget::kZoom = 45.0f;
const float QLocalOpenGLWidget::kNoFullScreenR = 8.5f;
const float QLocalOpenGLWidget::kFullScreenR = 12.0f;
const float QLocalOpenGLWidget::kMouseSensitivity = 0.005f;//鼠标灵敏度
const float QLocalOpenGLWidget::kMiddleMouseSensitivity = 0.03f;
const float QLocalOpenGLWidget::kWheelSensitivity = 0.01f;
const float QLocalOpenGLWidget::kCameraLocalAngle = -90.0f;
const float QLocalOpenGLWidget::kRotateUnitOffset = 30.0f;
//色域
const float QLocalOpenGLWidget::kDefaultColorGamut = 0.75;

QLocalOpenGLWidget::QLocalOpenGLWidget(QWidget * parent)
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
	keySPressEvent (false),
	keyAPressEvent(false),
    keyDPressEvent(false),
    pickMsg(-1.0f,70.0f),
    //coordinateAxisVisible(true),
	colorMarkVisible(true),
	full_screen_enable(false),
	camera_move_enable(false),
	is_context_init(false),
	worker(nullptr),
	thread(nullptr)
	//context(NULL)
{	

	this->scr_width = this->size().width();
	this->scr_height = this->size().height();
		
	//this->startTimer(TIMESPAN);//ms
	this->timer.start(TIMESPAN, this);

	//surface = new QOffscreenSurface;
	//surface->setFormat(ctx->format());
	//surface->setScreen(ctx->screen());
	//surface->create();

	//this->surface = new QSurface(QSurface::OpenGLSurface);
	//create opengl context
	//this->context = new QOpenGLContext;
	//this->context->setFormat(QSurfaceFormat::defaultFormat());
	//this->context->create();
}
//void QLocalOpenGLWidget::SetOpenGLContext(QOpenGLContext *context)
//{
//	if (context == NULL)
//	{
//		std::cout << "Error::QLocalOpenGLWidget::SetOpenGLContext::empty context pointer" << std::endl;
//		return;
//	}
//	if (!context->isValid())
//	{
//		std::cout << "Error::QLocalOpenGLWidget::SetOpenGLContext::invalid context" << std::endl;
//		return;
//	}
//}
QLocalOpenGLWidget::~QLocalOpenGLWidget() { //这个函数要修改上下文，所以需要makeCurrent
	this->makeCurrent();

	this->doneCurrent();
}


void QLocalOpenGLWidget::InitShaders()
{
	 //shaderProgram_other;//着色器
	 //shaderProgram_flange_tread;
	shaderProgram_other.initShaderProgram(FilePath::kVWheelOtherShaderPath,
       FilePath::kFWheelOtherShaderPath);
	shaderProgram_flange_tread.initShaderProgram(FilePath::kVWheelFlangeTreadShaderPath,
		FilePath::kFWheelFlangeTreadShaderPath);

	line_shader_program.initShaderProgram(FilePath::kVLineShaderPath, FilePath::kFLineShaderPath);

	font_shader_program.initShaderProgram(FilePath::kVFontShaderPath, FilePath::kFFontShaderPath);
}
void QLocalOpenGLWidget::initializeGL()
{
	//this->context->makeCurrent(this->Window);

	initializeOpenGLFunctions();//glad

	glViewport(0, 0, scr_width, scr_height);

	// Enable depth buffer
	//glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    InitShaders();
	
	//initEngine();
	//timer.start(12, this);
	this->LoadFontTexture();

	this->InitPolarCoordinate();
	this->InitCartesianCoordinate();
	//this->camera.SetDisToFocus(15.0f);
	this->InitCamera();
    //InitCoordinateAxis();
	//std::cout << "Start" << std::endl;
	//this->AddDefaultStandardWheelMaker();

    //ReadTxtMsg();//他调用了doneCurrent函数！不能放在前面！

	//std::cout << "End" << std::endl;
	InitShaderValue();

	//std::cout << "Ok" << std::endl;
	//this->camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	this->ContextIsInit();
}
void QLocalOpenGLWidget::ContextIsInit()
{
	//std::cout << "QLocalOpenGLWidget::ContextIsInit::init over" << std::endl;
	this->is_context_init = true;
	if (this->worker != nullptr)
	{
		//std::cout << "QLocalOpenGLWidget::ContextIsInit::IN?" << std::endl;
		this->worker->set_context_init_state(this->is_context_init);
	}
	emit InitGLOver();
}
void QLocalOpenGLWidget::LoadFontTexture()
{
	this->font_texture = Font::LoadFontTexture();
	font_shader_program.use();
	font_shader_program.setInt("font_texture", 0);//GL_TEXTURE0
}
void QLocalOpenGLWidget::InitPolarCoordinate()
{
	this->makeCurrent();
	Coordinate::PolarCoordinate polar_coordinate(500.0f);
	float angle_mark_text_size = 20;
	float circle_mark_text_size = 15;
	glm::vec3 angle_mark_text_color = glm::vec3(0.0f, 1.0f, 1.0f);
	glm::vec3 circle_mark_text_color = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 coordinate_line_color = glm::vec3(0.0f, 0.0f, 1.0f);
	std::string circle_unit = "mm";

	polar_coordinate.set_color(coordinate_line_color);

	polar_coordinate.AddAngleMark(Coordinate::Mark(0.0f/360.0f, coordinate_line_color,Text("0", angle_mark_text_size, angle_mark_text_color)));
	polar_coordinate.AddAngleMark(Coordinate::Mark(45.0f / 360.0f, coordinate_line_color, Text("45", angle_mark_text_size, angle_mark_text_color)));
	polar_coordinate.AddAngleMark(Coordinate::Mark(90.0f / 360.0f, coordinate_line_color, Text("90", angle_mark_text_size, angle_mark_text_color)));
	polar_coordinate.AddAngleMark(Coordinate::Mark(135.0f / 360.0f, coordinate_line_color, Text("135", angle_mark_text_size, angle_mark_text_color)));
	polar_coordinate.AddAngleMark(Coordinate::Mark(180.0f / 360.0f, coordinate_line_color, Text("180", angle_mark_text_size, angle_mark_text_color)));
	polar_coordinate.AddAngleMark(Coordinate::Mark(225.0f / 360.0f, coordinate_line_color, Text("225", angle_mark_text_size, angle_mark_text_color)));
	polar_coordinate.AddAngleMark(Coordinate::Mark(270.0f / 360.0f, coordinate_line_color, Text("270", angle_mark_text_size, angle_mark_text_color)));
	polar_coordinate.AddAngleMark(Coordinate::Mark(315.0f / 360.0f, coordinate_line_color, Text("315", angle_mark_text_size, angle_mark_text_color)));

	polar_coordinate.AddCircleMark(Coordinate::Mark(0.0f / 500.0f, coordinate_line_color, Text("0"+ circle_unit, circle_mark_text_size, circle_mark_text_color)));
	polar_coordinate.AddCircleMark(Coordinate::Mark(100.0f / 500.0f, coordinate_line_color, Text("100" + circle_unit, circle_mark_text_size, circle_mark_text_color)));
	polar_coordinate.AddCircleMark(Coordinate::Mark(200.0f / 500.0f, coordinate_line_color, Text("200" + circle_unit, circle_mark_text_size, circle_mark_text_color)));
	polar_coordinate.AddCircleMark(Coordinate::Mark(300.0f / 500.0f, coordinate_line_color, Text("300" + circle_unit, circle_mark_text_size, circle_mark_text_color)));
	polar_coordinate.AddCircleMark(Coordinate::Mark(400.0f / 500.0f, coordinate_line_color, Text("400" + circle_unit, circle_mark_text_size, circle_mark_text_color)));
	polar_coordinate.AddCircleMark(Coordinate::Mark(500.0f / 500.0f, coordinate_line_color, Text("500" + circle_unit, circle_mark_text_size, circle_mark_text_color)));

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));

	glm::mat4 coordinate_model(1.0f);
	coordinate_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -135.0f))*coordinate_model;
	coordinate_model = scale*coordinate_model;
	//coordinate_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*coordinate_model;
	//coordinate_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))*coordinate_model;
	//coordinate_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.4f, 0.0f))*coordinate_model;

	//std::cout << "h_ruler!" << std::endl;
	auto polar_coordinate_object = polar_coordinate.LoadObject(this->font_texture);
	polar_coordinate_object.SetModel(coordinate_model);

	engine.AddPolarCoordinate(polar_coordinate_object);

	this->doneCurrent();
}
void QLocalOpenGLWidget::InitCartesianCoordinate()
{
	this->makeCurrent();

	float axis_text_size = 20;
	glm::vec3 x_color = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 y_color = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 z_color = glm::vec3(1.0f, 0.0f, 0.0f);

	Coordinate::CartesianCoordinate coordinate;
	coordinate.set_axis_length(500.0f, 500.0f, 500.0f);
	coordinate.set_axis_text_dis_to_axis_end(20.0f);
	coordinate.set_axis_extend(100.0f);
	coordinate.set_coordinate_octant(Coordinate::kCoordinateWholeOctant);
	coordinate.set_axis_color(x_color,y_color,z_color);
	coordinate.set_axis_text(Text("X", axis_text_size, x_color), Text("Y", axis_text_size, y_color), Text("Z", axis_text_size, z_color));

	/*coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(70.0f / 135.0f, Text("70", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(135.0f / 135.0f, Text("135", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));

	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(35.0f / 135.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));
	coordinate.AddMark(Coordinate::Axis::X, Coordinate::Mark(105.0f / 135.0f, Text("", coordinate_text_size, glm::vec3(1.0f, 1.0f, 1.0f))));*/


	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));

	glm::mat4 coordinate_model(1.0f);
	coordinate_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -135.0f))*coordinate_model;
	coordinate_model = scale*coordinate_model;
	//coordinate_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f))*coordinate_model;
	//coordinate_model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f))*coordinate_model;
	//coordinate_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.4f, 0.0f))*coordinate_model;

	//std::cout << "h_ruler!" << std::endl;
	auto coordinate_object = coordinate.LoadObject(this->font_texture);
	coordinate_object.SetModel(coordinate_model);

	engine.AddCartesianCoordinate(coordinate_object);

	this->doneCurrent();
}
void QLocalOpenGLWidget::InitShaderValue()
{
	this->makeCurrent();
	//shaderProgram_other.setFloat("pickRadian", pickMsg.GetRadian());
	//shaderProgram_other.setFloat("pickWidth", pickMsg.GetWidth());
	//haderProgram_other.setBool("redMark",this->redMarkVisible);
	shaderProgram_flange_tread.use();
	shaderProgram_flange_tread.setFloat("colorMark", this->colorMarkVisible);
	//shaderProgram_flange_tread.setFloat("pickRadian", pickMsg.GetRadian());
	//shaderProgram_flange_tread.setFloat("pickWidth", pickMsg.GetWidth());

	this->doneCurrent();
}
void QLocalOpenGLWidget::InitCoordinateAxis()
{
    //std::cout<<"1"<<std::endl;
    LoadModel loadModel;
    Object::MeshObject Arrow(loadModel.loadModel(std::string(FilePath::kArrowPath)));
	Object::MeshObject Line(loadModel.loadModel(std::string(FilePath::kLinePath)));
    //std::cout<<"2"<<std::endl;
    this->engine.SetCoordinateAxis(Arrow,Line);
    //std::cout<<"3"<<std::endl;
}
void QLocalOpenGLWidget::ReadTxtMsg()
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
			surveyTread.insert(startX+0.1f*j,circlePoint);
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
	while (file >>x >> y)
	{
		surveyFlange.push_back(QVector2D(x,y));
	}
	file.close();

	//std::cout << "Start1" << std::endl;
	this->AddTestWheel(surveyFlange, surveyTread, FilePath::k_FloatPlant_SH840_4L_CadPath);
	//this->AddWheel(surveyFlange, surveyTread,"W:\\CarWheel\\DrawCarWheelTest\\DrawCarWheelTest\\Resources\\CadMsg\\SH840-4L.txt");
}
//切换对比数据
void QLocalOpenGLWidget::ReplaceStandardWheelMsg(const DxfReader::StandardFlangeTreadMsgMaker& msg_maker)
{
	if (msg_maker.empty())	{
		this->wheel_color_contrasts[this->currentWheelId].set_standard_tread(std::vector<glm::vec2>());//传空
		this->UpdateStandardFlangeTreadAndColorModelForShader();//更新状态
		return;
	}

	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())
	{
		return;
	}
	else
	{
		//std::cout << "QLocalOpenGLWidget::ReplaceStandardWheelMsg::OK?" << std::endl;
		auto st_msg = this->wheel_flange_treads[this->currentWheelId].LoadStandardTread(msg_maker);
		//std::cout << "QLocalOpenGLWidget::ReplaceStandardWheelMsg::OK??" << std::endl;
		this->wheel_color_contrasts[this->currentWheelId].set_standard_tread(st_msg);
		//std::cout << "QLocalOpenGLWidget::ReplaceStandardWheelMsg::OK???" << std::endl;
		//this->SetCurrentWheel(this->currentWheelId);//更新当前状态,将新的标准点赋值给shader
		this->UpdateStandardFlangeTreadAndColorModelForShader();
		//std::cout << "QLocalOpenGLWidget::ReplaceStandardWheelMsg::OK????" << std::endl;
	}

	//if (file_path == "")
	//{
	//	this->wheel_color_contrasts[this->currentWheelId].set_standard_tread(std::vector<glm::vec2>());//传空

	//	return;
	//}

	////if (this->standard_wheel_makers.find(file_path) == this->standard_wheel_makers.end())
	////{
	////	Wheel::StandardFlangeTreadMsgMake msg_make(file_path);//数据读取错误会报错
	////	this->standard_wheel_makers.insert({ file_path,msg_make });
	////}

	//if (this->currentWheelId == -1)
	//{
	//	return;
	//}
	//else
	//{
	//	auto st_msg = this->wheel_flange_treads[this->currentWheelId].LoadStandardTread(this->standard_wheel_makers[file_path]);
	//	this->wheel_color_contrasts[this->currentWheelId].set_standard_tread(st_msg);
	//	//this->SetCurrentWheel(this->currentWheelId);//更新当前状态,将新的标准点赋值给shader
	//	this->UpdateStandardFlangeTreadAndColorModelForShader();
	//}
}
void QLocalOpenGLWidget::SetColorGamut(float color_gamut)
{
	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())
	{
		return;
	}

	int st_size = this->wheel_color_contrasts[this->currentWheelId].get_standard_tread().size();

	if (st_size == 0)
	{
		return;
	}
	else
	{
		if (color_gamut <= 0)
		{
			std::cout << "Error::QLocalOpenGLWidget::SetColorGamut::illegal color gamut" << std::endl;
			return;
		}
		else
		{
			this->wheel_color_contrasts[this->currentWheelId].set_color_gamut(color_gamut);
			this->UpdateColorGamutForShader();
		}
	}
}
void QLocalOpenGLWidget::UpdateFloatPlant(std::string float_plant_file_path, const DxfReader::FloatPlantMsgMaker& msg_maker)
{
	if (msg_maker.empty())
	{
		std::cout << "Error::QLocalOpenGLWidget::UpdateFloatPlan::illegal msg_maker" << std::endl;
		return;
	}

	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
	glm::vec3 color(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f);

	//std::cout << "QLocalOpenGLWidget::UpdateFloatPlant" << std::endl;
	//std::cout << "size:" << this->floatPlants.size() << std::endl;
	//std::cout << "Add FP Path:" << float_plant_file_path << std::endl;
	if (this->floatPlants.find(float_plant_file_path) == this->floatPlants.end())//新的
	{
		//std::cout << "1" << std::endl;
		//std::cout << "QLocalOpenGLWidget::UpdateFloatPlant::OK!" << std::endl;
		std::vector<glm::vec2> floatPlant = msg_maker.GetFloatPlantPoints(Wheel::WheelFlangeTread::kStandardTreadRadius);

		//std::cout << "Float plant msg:" << std::endl;
		//for (int i = 0; i != floatPlant.size(); ++i)
		//{
		//	//std::cout
		//}
		//std::cout << "QLocalOpenGLWidget::UpdateFloatPlant::OK!!" << std::endl;

		this->floatPlants.insert({ float_plant_file_path,floatPlant });
		//std::cout << "FloatPlant size:" << floatPlant.size() << std::endl;
		OpenglObj::OpenglPointMsg floatPlantMsg = Wheel::LoadFloatPlantPoint(floatPlant);
		//std::cout << "QLocalOpenGLWidget::UpdateFloatPlant::OK!!!" << std::endl;

		this->makeCurrent();
		//std::cout << "QLocalOpenGLWidget::UpdateFloatPlant::Fuck" << std::endl;
		//this->initializeOpenGLFunctions();
		//QOpenGLContext::currentContext();

		auto mesh = OpenglObj::Mesh(floatPlantMsg.GetVertices(), floatPlantMsg.GetIndices());
		//std::cout << "QLocalOpenGLWidget::UpdateFloatPlant::OK!!!!" << std::endl;
		auto float_plant_obj = Object::MeshObject(mesh, model, color);
		//std::cout << "QLocalOpenGLWidget::UpdateFloatPlant::OK!!!!!" << std::endl;
		this->engine.AddWheelFloatPlant(float_plant_file_path, float_plant_obj/*,floatPlant,this->wheel_flange_treads*/);

		this->doneCurrent();
	}
	else //旧的，要更新connectObj
	{
		std::vector<glm::vec2> floatPlant = msg_maker.GetFloatPlantPoints(Wheel::WheelFlangeTread::kStandardTreadRadius);
		this->floatPlants.insert({ float_plant_file_path,floatPlant });
		OpenglObj::OpenglPointMsg floatPlantMsg = Wheel::LoadFloatPlantPoint(floatPlant);
		this->makeCurrent();
		this->engine.UpdateFloatPlant(float_plant_file_path, Object::MeshObject(OpenglObj::Mesh(floatPlantMsg.GetVertices(), floatPlantMsg.GetIndices()), model, color), floatPlant, this->wheel_flange_treads);
		this->doneCurrent();
	}
}
void QLocalOpenGLWidget::UpdateColorGamutForShader()
{
	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())
	{
		std::cout << "Error::QLocalOpenGLWidget::UpdateColorGamutForShader::illegal currentWheelId";
		return;
	}

	int st_size = this->wheel_color_contrasts[this->currentWheelId].get_standard_tread().size();
	if (st_size != 0)
	{
		//刷新shader
		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("color_gamut", this->wheel_color_contrasts[currentWheelId].get_color_gamut());
		this->doneCurrent();
	}
	else
	{
		std::cout << "Error::QLocalOpenGLWidget::UpdateColorGamutForShader::update color gamut in height contrast" << std::endl;
	}
}
void QLocalOpenGLWidget::SwitchFloatPlant(std::string float_plant_file_path)
{
	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())
	{
		std::cout << "Warning::QLocalOpenGLWidget::SwitchFloatPlant::currentWheelId == -1" << std::endl;
		return;
	}
	if (this->floatPlants.find(float_plant_file_path) == this->floatPlants.end())
	{
		std::cout << "Error::QLocalOpenGLWidget::SwitchFloatPlant::illegal file_path" << std::endl;
		return;
	}
	if (this->engine.GetCurrentFloatPlantPath() == float_plant_file_path)
	{
		std::cout << "Warning::QLocalOpenGLWidget::SwitchFloatPlant::Same Float plant" << std::endl;
		return;
	}

	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
	glm::vec3 color(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f);

	this->makeCurrent();
	//切换轮子要切换当前wheelObject的floatplantfilepath和wheelconnectObj
	OpenglObj::OpenglPointMsg wheelConnectionMsg = Wheel::LoadConnectionPoint(this->wheel_flange_treads[currentWheelId].get_flange_tread(), this->floatPlants[float_plant_file_path]);
	this->engine.SwitchFloatPlant(float_plant_file_path, Object::MeshObject(OpenglObj::Mesh(wheelConnectionMsg.GetVertices(), wheelConnectionMsg.GetIndices()), model, color));
	this->doneCurrent();
}
void QLocalOpenGLWidget::CreateThread()
{
	if (this->thread!=nullptr)
	{
		return;
	}
	this->thread = new QThread();
	this->worker = new QLocalOpenGLWidgetThread(this->is_context_init);
	this->worker->moveToThread(this->thread);

	connect(this->thread, &QThread::finished, this->thread, &QObject::deleteLater);
	connect(this->thread, &QThread::finished, this->worker, &QObject::deleteLater);

	connect(this, &QLocalOpenGLWidget::StartAddWheelThread, this->worker, &QLocalOpenGLWidgetThread::Run);
	//connect(this, &QLocalOpenGLWidget::startObjThreadWork2, m_obj, &ThreadObject::runSomeBigWork2);
	connect(this->worker, &QLocalOpenGLWidgetThread::ProgressLog, this, &QLocalOpenGLWidget::ProcessLog);
	connect(this->worker, &QLocalOpenGLWidgetThread::ErrorLog, this, &QLocalOpenGLWidget::ErrorLog);
	connect(this->worker, static_cast<void (QLocalOpenGLWidgetThread::*)(int,const OpenglObj::OpenglPointMsg&,const OpenglObj::OpenglPointMsg&,const std::string&,bool, const OpenglObj::OpenglPointMsg& )>(&QLocalOpenGLWidgetThread::Result), this, &QLocalOpenGLWidget::AddOpenglMsg);
	connect(this->worker, static_cast<void(QLocalOpenGLWidgetThread::*)(int)>(&QLocalOpenGLWidgetThread::Result), this, &QLocalOpenGLWidget::SetCurrentWheel);
	this->thread->start();
}
void QLocalOpenGLWidget::SetWheel(const QString& wheel_id, const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, FilePath::FloatPlant float_plant)
{
	/*
	if (this->wheel_ids.find(wheel_id.toStdString()) != this->wheel_ids.end())
	{
		this->SetCurrentWheel(this->wheel_ids[wheel_id.toStdString()]);
		return;
	}

	Wheel::WheelFlangeTread wheel(surveyFlange, surveyTread);
	this->wheel_flange_treads.push_back(wheel);

	//std::vector<Wheel::WheelFlangeTread>& a;
	//std::vector<Wheel::WheelColorContrast>& b;
	//std::map<std::string, std::vector<glm::vec2>>& c;
	//添加颜色对比
	this->wheel_color_contrasts.push_back(Wheel::WheelColorContrast(kDefaultColorGamut, wheel.get_flange_tread_r_range(), std::vector<glm::vec2>()));
	
	if (FilePath::GetFloatPlantPath(float_plant) == NULL)
	{
		std::cout << "Error::QLocalOpenGLWidget::SetWheel::illegal float_plant" << std::endl;
		return;
	}

	QDir temDir(FilePath::GetFloatPlantPath(float_plant));
	QString absDir = temDir.absolutePath();
	std::string float_plant_path = absDir.toStdString();

	bool new_float_plant = false;
	OpenglObj::OpenglPointMsg floatPlantMsg;
	if (this->floatPlants.find(float_plant_path) == this->floatPlants.end())//找不到filePath,添加
	{
		std::string error_log = std::string("There are some problem when read this float plant file:\n") + float_plant_path + " :\n";

		DxfReader::DxfReader* reader = new DxfReader::DxfReader();
		DL_Dxf* dxf = new DL_Dxf();
		if (!dxf->in(float_plant_path, reader)) { // if file open failed
			std::cerr << float_plant_path << " could not be opened.\n";
			error_log += std::string("Error::This file could not be opened.\n");
			QMessageBox::warning(NULL, "Read Float Plant Error Log", QString(error_log.c_str()), QMessageBox::Ok);//log
			return;
		}

		auto msg_maker = reader->LoadFPMsgMaker();

		if (reader->get_error_log() != "")//弹出log
		{
			QMessageBox::warning(NULL, "Read Float Plant Error Log", QString((error_log + reader->get_error_log()).c_str()), QMessageBox::Ok);
			return;
		}

		delete dxf;
		delete reader;

		if (msg_maker.empty())
		{
			std::cout << "QLocalOpenGLWidget::SetWheel::empty msg_maker" << std::endl;
			return;
		}

		std::vector<glm::vec2> floatPlant = msg_maker.GetFloatPlantPoints(Wheel::WheelFlangeTread::kStandardTreadRadius);
		this->floatPlants.insert({ float_plant_path,floatPlant });
		floatPlantMsg = Wheel::LoadFloatPlantPoint(floatPlant);
		new_float_plant = true;
	}

	//添加wheelObject,用表中的第一个作为默认path
	OpenglObj::OpenglPointMsg wheelFlangeTreadMsg = wheel.LoadFlangeTreadPoint();
	OpenglObj::OpenglPointMsg wheelConnectionMsg = Wheel::LoadConnectionPoint(wheel.get_flange_tread(), this->floatPlants[float_plant_path]);

	this->wheel_ids.insert({ wheel_id.toStdString(),this->wheel_flange_treads.size() - 1 });

	//添加到opengl的数据
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放

	glm::vec3 color(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f);

	this->makeCurrent();
	if (new_float_plant)
	{
		this->engine.UpdateFloatPlant(float_plant_path, Object::MeshObject(OpenglObj::Mesh(floatPlantMsg.GetVertices(), floatPlantMsg.GetIndices()), model, color), this->floatPlants[float_plant_path], this->wheel_flange_treads);
	}
	this->engine.AddWheelObject(
		WheelObject(Object::MeshObject(OpenglObj::Mesh(wheelFlangeTreadMsg.GetVertices(), wheelFlangeTreadMsg.GetIndices()), model, color),//flangeTreadObj
			Object::MeshObject(OpenglObj::Mesh(wheelConnectionMsg.GetVertices(), wheelConnectionMsg.GetIndices()), model, color),//connectionObj
			float_plant_path)
	);
	this->doneCurrent();


	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())//默认currentWheelID
	{
		this->SetCurrentWheel(0);
	}
	*/

	//std::cout << "QLocalOpenGLWidget::SetWheel::1" << std::endl;
	if (this->thread==nullptr)
	{
		this->CreateThread();
	}
	//std::cout << "QLocalOpenGLWidget::SetWheel::2" << std::endl;
	emit StartAddWheelThread(wheel_id,surveyFlange,surveyTread,float_plant,&this->wheel_ids, &this->wheel_flange_treads, &this->wheel_color_contrasts, &this->floatPlants);//主线程通过信号换起子线程的槽函数
}
void QLocalOpenGLWidget::AddOpenglMsg(int wheel_id,const OpenglObj::OpenglPointMsg &wheelFlangeTreadMsg, const OpenglObj::OpenglPointMsg& wheelConnectionMsg,const std::string& float_plant_path,bool new_float_plant,const OpenglObj::OpenglPointMsg& floatPlantMsg)
{
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放

	glm::vec3 color(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f);

	//添加到opengl的数据
	this->makeCurrent();
	if (new_float_plant)
	{
		this->engine.UpdateFloatPlant(float_plant_path, Object::MeshObject(OpenglObj::Mesh(floatPlantMsg.GetVertices(), floatPlantMsg.GetIndices()), model, color), this->floatPlants[float_plant_path], this->wheel_flange_treads);
	}

	this->engine.AddWheelObject(
		WheelObject(Object::MeshObject(OpenglObj::Mesh(wheelFlangeTreadMsg.GetVertices(), wheelFlangeTreadMsg.GetIndices()), model, color),//flangeTreadObj
			Object::MeshObject(OpenglObj::Mesh(wheelConnectionMsg.GetVertices(), wheelConnectionMsg.GetIndices()), model, color),//connectionObj
			float_plant_path)
	);
	this->doneCurrent();

	if (wheel_id!=0/*wheel_id < 0 || wheel_id >= this->wheel_flange_treads.size()*/)//默认currentWheelID
	{
		std::cout << "Error::QLocalOpenGLWidget::AddOpenglMsg::illegal wheel id" << std::endl;
		return;
	}
	else
	{
		this->SetCurrentWheel(wheel_id);
	}

}
//这个函数示意，所有要添加floatplant
void QLocalOpenGLWidget::AddTestWheel(const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, const std::string& default_float_plant_file_path)
{

	this->makeCurrent();
	//std::cout << "QLocalOpenGLWidget::AddTestWheel" << std::endl;
	//std::cout << "Start" << std::endl;
	Wheel::WheelFlangeTread wheel(surveyFlange, surveyTread/*, filePath*/);
	this->wheel_flange_treads.push_back(wheel);

	//添加颜色对比
	this->wheel_color_contrasts.push_back(Wheel::WheelColorContrast(kDefaultColorGamut, wheel.get_flange_tread_r_range(), std::vector<glm::vec2>()));

    //emit PickWidthRange(wheel.GetTreadRange().GetStart(),wheel.GetTreadRange().GetEnd());
	emit SpreadWheelOpenGLMsg(wheel.LoadSpreadFlangeTreadPoint());

	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放

	glm::vec3 color(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f);

	//为例子数据先添加floatplant
	//获得绝对路径
	QDir temDir(default_float_plant_file_path.c_str());
	QString absDir = temDir.absolutePath();
	std::string float_plant_path = absDir.toStdString();

	//std::cout << "Test WHeel path:" << float_plant_path <<std::endl;
	//std::cout << "Test Wheel:" << this->floatPlants.size() << std::endl;
	if (this->floatPlants.find(float_plant_path)==this->floatPlants.end())//找不到filePath,添加
	{
		DxfReader::DxfReader* reader = new DxfReader::DxfReader();
		DL_Dxf* dxf = new DL_Dxf();
		if (!dxf->in(float_plant_path, reader)) { // if file open failed
			std::cerr << float_plant_path << " could not be opened.\n";
			return;
		}

		auto msg_maker = reader->LoadFPMsgMaker();

		delete dxf;
		delete reader;

		if (msg_maker.empty())
		{
			std::cout << "QLocalOpenGLWidget::AddTestWheel::empty msg_maker" << std::endl;
			return;
		}

		std::vector<glm::vec2> floatPlant = /*Wheel::InitFloatPlate(float_plant_path)*/msg_maker.GetFloatPlantPoints(Wheel::WheelFlangeTread::kStandardTreadRadius);
		this->floatPlants.insert({ float_plant_path,floatPlant });
		OpenglObj::OpenglPointMsg floatPlantMsg = Wheel::LoadFloatPlantPoint(floatPlant);
		//std::cout << "End" << std::endl;

		this->engine.UpdateFloatPlant(float_plant_path, Object::MeshObject(OpenglObj::Mesh(floatPlantMsg.GetVertices(), floatPlantMsg.GetIndices()),model,color), floatPlant, this->wheel_flange_treads);
	}

	//添加wheelObject
	OpenglObj::OpenglPointMsg wheelFlangeTreadMsg = wheel.LoadFlangeTreadPoint();
	OpenglObj::OpenglPointMsg wheelConnectionMsg = Wheel::LoadConnectionPoint(wheel.get_flange_tread(), this->floatPlants[float_plant_path]);
	this->engine.AddWheelObject(
		WheelObject(Object::MeshObject(OpenglObj::Mesh(wheelFlangeTreadMsg.GetVertices(), wheelFlangeTreadMsg.GetIndices()), model, color),//flangeTreadObj
			Object::MeshObject(OpenglObj::Mesh(wheelConnectionMsg.GetVertices(), wheelConnectionMsg.GetIndices()), model, color),//connectionObj
			float_plant_path)
	);

	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())//默认currentWheelID
	{
		this->SetCurrentWheel(0);
	}

    this->doneCurrent();  //注意doneCurrent!
}

void QLocalOpenGLWidget::AddWheel(const Wheel::WheelFlangeTread& wft_msg, std::string default_float_plant_file_path)
{
	this->makeCurrent();

	//Wheel::WheelFlangeTread wheel(surveyFlange, surveyTread/*, filePath*/);
	this->wheel_flange_treads.push_back(wft_msg);

	//添加颜色对比
	this->wheel_color_contrasts.push_back(Wheel::WheelColorContrast(kDefaultColorGamut, wft_msg.get_flange_tread_r_range(), std::vector<glm::vec2>()));

	//emit PickWidthRange(wheel.GetTreadRange().GetStart(),wheel.GetTreadRange().GetEnd());
	//emit SpreadWheelOpenGLMsg(wheel.LoadSpreadFlangeTreadPoint());

	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放

	glm::vec3 color(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f);

	//if (this->floatPlants.find(filePath) == this->floatPlants.end())//找不到filePath,添加
	//{
	//	std::vector<glm::vec2> floatPlant = Wheel::InitFloatPlate(filePath);
	//	this->floatPlants.insert({ filePath,floatPlant });
	//	OpenglObj::OpenglPointMsg floatPlantMsg = Wheel::LoadFloatPlantPoint(floatPlant);
	//	this->engine.AddWheelFloatPlant(filePath, Object::MeshObject(OpenglObj::Mesh(floatPlantMsg.GetVertices(), floatPlantMsg.GetIndices()), model, color));
	//}

	if (this->floatPlants.size() == 0)
	{
		std::cout << "ERROR::QLocalOpenGLWidget::AddWheel::NO Float plant to be add" << std::endl;
		return;
	}

	std::string path;
	if (default_float_plant_file_path == "")
	{
		path = this->floatPlants.begin()->first;
	}
	else
	{
		if (this->floatPlants.find(default_float_plant_file_path) == this->floatPlants.end())
		{
			std::cout << "Error::QLocalOpenGLWidget::AddWheel No Find this Default file path" << std::endl;
			return;
		}
		else
		{
			path = default_float_plant_file_path;
		}
	}

	//添加wheelObject,用表中的第一个作为默认path
	OpenglObj::OpenglPointMsg wheelFlangeTreadMsg = wft_msg.LoadFlangeTreadPoint();
	OpenglObj::OpenglPointMsg wheelConnectionMsg = Wheel::LoadConnectionPoint(wft_msg.get_flange_tread(), this->floatPlants[path]);
	this->engine.AddWheelObject(
		WheelObject(Object::MeshObject(OpenglObj::Mesh(wheelFlangeTreadMsg.GetVertices(), wheelFlangeTreadMsg.GetIndices()), model, color),//flangeTreadObj
			Object::MeshObject(OpenglObj::Mesh(wheelConnectionMsg.GetVertices(), wheelConnectionMsg.GetIndices()), model, color),//connectionObj
			path)
	);

	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())//默认currentWheelID
	{
		this->SetCurrentWheel(0);
	}

	this->doneCurrent();  //注意doneCurrent!
}
void QLocalOpenGLWidget::UpdateWheelFlangeTreadModel(float radian)
{
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));//缩放
	//...
	model = glm::rotate(model, radian, glm::vec3(0.0f, 0.0f, -1.0f));
	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())
	{
		std::cout << "Error::QLocalOpenGLWidget::UpdateWheelFlangeTreadModel::illegal id!" << std::endl;
	}
	else
	{
		this->engine.UpdateWheelFlangeTreadModel(model, this->currentWheelId);
	}
}
//对于wheel_spread_shader_program的更新
void QLocalOpenGLWidget::UpdateStandardFlangeTreadAndColorModelForShader()
{
	if (this->currentWheelId < 0 || this->currentWheelId >= this->wheel_flange_treads.size())
	{
		std::cout << "Error::QLocalOpenGLWidget::UpdateStandardFlangeTreadAndColorModelForShader::illegal currentWheelId";
		return;
	}

	int size = this->wheel_color_contrasts[currentWheelId].get_standard_tread().size();

	this->makeCurrent();
	shaderProgram_flange_tread.use();
	if (size == 0)
	{
		shaderProgram_flange_tread.setInt("color_model", 0);
	}
	else
	{
		shaderProgram_flange_tread.setInt("color_model", 1);
		for (int i = 0; i != size; ++i)
		{
			std::string name = "standardTreadMsg[" + std::to_string(i) + "]";
			shaderProgram_flange_tread.setVec2(name, this->wheel_color_contrasts[currentWheelId].get_standard_tread()[i]);
		}
		shaderProgram_flange_tread.setInt("standardTreadMsgSize", size);
	}
	this->doneCurrent();
}
//void QLocalOpenGLWidget::AddDefaultStandardWheelMaker()
//{//相对路径和绝对路径？
//	if (this->standard_wheel_makers.find(FilePath::kExampleStandardTreadPath) != this->standard_wheel_makers.end())
//	{
//		std::cout << "Error::QLocalSpreadOpenglWidget::AddDefaultStandardWheelMaker::this path is already exist" << std::endl;
//		return;
//	}
//	Wheel::StandardFlangeTreadMsgMake msg_make(FilePath::kExampleStandardTreadPath);
//	this->standard_wheel_makers.insert({ FilePath::kExampleStandardTreadPath,msg_make });
//}
void QLocalOpenGLWidget::SetCurrentWheel(int id)
{
	if (id < 0 || id >= this->wheel_flange_treads.size())
	{
		std::cout << "ERROR::LOCALWIDGET::SETCURRENTWHEEL::ILLEGAL ID!" << std::endl;
	}
	else
	{
		this->engine.SetCurrentWheelId(id);
		this->currentWheelId = id;
		pickMsg.SetRadian(0.0f);
		pickMsg.SetWidth(70.0f);

		emit PickSectionMsg(wheel_flange_treads[this->currentWheelId].GetSection(0.0f));
		//emit PickSectionRadian(0);

		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("pickRadian", pickMsg.GetLength());
		shaderProgram_flange_tread.setFloat("pickWidth", pickMsg.GetWidth());

		shaderProgram_flange_tread.setFloat("treadR", Wheel::WheelFlangeTread::kStandardTreadRadius);

		shaderProgram_flange_tread.setFloat("widthMax", 105.0f);
		shaderProgram_flange_tread.setFloat("max_r", this->wheel_color_contrasts[currentWheelId].get_flange_tread_r_range().GetMax());
		shaderProgram_flange_tread.setFloat("min_r", this->wheel_color_contrasts[currentWheelId].get_flange_tread_r_range().GetMin());
		shaderProgram_flange_tread.setFloat("color_gamut", this->wheel_color_contrasts[currentWheelId].get_color_gamut());

		this->doneCurrent();

		//this->ReplaceStandardWheelMsg();
		this->UpdateStandardFlangeTreadAndColorModelForShader();
		//this->engine.SetCurrentWheel(id);
		this->InitCamera();
	}
}
int QLocalOpenGLWidget::GetWheelSize()
{
    return this->engine.GetWheelsSize();
}
//void QLocalOpenGLWidget::initEngine()
//{
//	Wheel wheel;
//	Mesh wheelMesh(wheel.GetVertices(), wheel.GetIndices());  //wheelMesh(std::vector<Vertex>(),std::vector<unsigned int>())不行？
//	std::cout << wheel.GetVertices().size() << std::endl;
//	glm::mat4 model(1.0f);
//	model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));  //不允许 非y轴360度 的翻转
//	this->engine.addMesh(wheelMesh, model);
//}
void QLocalOpenGLWidget::resizeGL(int w, int h)
{
	//int minValue = min(w, h);
	this->scr_width = w;
	this->scr_height = h;
	glViewport(0, 0, w, h);
	
}
//void QLocalOpenGLWidget::showEvent(QShowEvent *e)
//{
//	QOpenGLWidget::showEvent(e);
//	//this->InitPickValue();
//}
void QLocalOpenGLWidget::paintGL()
{
	QPainter painter(this);//不用paint画线是黑的

	painter.beginNativePainting();

	this->DrawOpengl();

	painter.endNativePainting();
}
void QLocalOpenGLWidget::DrawOpengl()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//设定清屏所用的颜色
										 //glClear(GL_COLOR_BUFFER_BIT);//接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram_flange_tread.use();
	shaderProgram_flange_tread.setVec3("objectColor", 192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
	shaderProgram_flange_tread.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaderProgram_flange_tread.setVec3("lightPos", camera.position);
	shaderProgram_flange_tread.setVec3("viewPos", camera.position);

	//view/projection
	view = camera.GetViewMatrix();//3d摄像机
	shaderProgram_flange_tread.setMat4("view", view);
	//glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(camera.zoom), ((float)this->scr_width / this->scr_height), 0.1f, 100.0f);
	shaderProgram_flange_tread.setMat4("projection", projection);

	if (this->currentWheelId ==0)
	{
		engine.DrawWheelFlangeTread(shaderProgram_flange_tread, this->currentWheelId);
	}

	shaderProgram_other.use();
	shaderProgram_other.setVec3("objectColor", 192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f);
	shaderProgram_other.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaderProgram_other.setVec3("lightPos", camera.position);
	shaderProgram_other.setVec3("viewPos", camera.position);
	shaderProgram_other.setMat4("view", view);
	shaderProgram_other.setMat4("projection", projection);
	if (this->currentWheelId ==0)
	{
		engine.DrawWheelFloatPlantAndConnection(shaderProgram_other, this->currentWheelId);
	}

	line_shader_program.use();
	line_shader_program.setMat4("view", view);
	line_shader_program.setMat4("projection", projection);

	font_shader_program.use();
	font_shader_program.setMat4("view", view);
	font_shader_program.setMat4("projection", projection);

	engine.UpdateTextDirection(camera.front, camera.up, projection, view);

	if (this->currentWheelId == 0)
	{
		engine.DrawCartesianCoordinates(line_shader_program, font_shader_program);
		engine.DrawPolarCoordinates(line_shader_program, font_shader_program);
	}

	glDisable(GL_DEPTH_TEST);
}
void QLocalOpenGLWidget::Draw2DText(QPainter& painter)
{

}
void QLocalOpenGLWidget::SetPickRadian(float radian)
{
    if (this->currentWheelId ==0)
    {
		//std::cout << "ok?" << std::endl;
        if (radian >= 0 && radian <= 2 * Wheel::WheelFlangeTread::kPIR)
        {
            if(abs(this->pickMsg.GetRadian() -radian)>0.001)
            {
                this->pickMsg.SetRadian(radian);
                this->makeCurrent();
				shaderProgram_flange_tread.use();
				shaderProgram_flange_tread.setFloat("pickRadian", radian);  //是-1可以刷新数据
                this->doneCurrent();
                //std::cout << "pickRadian:" << msg.radian << std::endl;
                emit PickSectionMsg(wheel_flange_treads[this->currentWheelId].GetSection(radian));//没有东西不发数据
            }
			this->UpdateWheelFlangeTreadModel(radian);
        }
        else
        {
            std::cout << "ERROR::LOCALWIDGET::SetPickRadian::ILLEGAL RADIAN" << std::endl;

        }
    }
    else
    {
        std::cout << "ERROR::LOCALWIDGET::SetPickRadian::NO WHEEL TO SET PICK RADIAN!" << std::endl;
    }
}
void QLocalOpenGLWidget::SetPickAngle(float angle)
{
    if (this->currentWheelId ==0)
    {
        if (angle >= 0 && angle <= 360)
        {
            float radian=(2* Wheel::WheelFlangeTread::kPIR)*angle/360;
            if(abs(this->pickMsg.GetRadian()-radian)>0.001)
            {
                this->pickMsg.SetRadian(radian);
                this->makeCurrent();
				shaderProgram_flange_tread.use();
				shaderProgram_flange_tread.setFloat("pickRadian", radian);  //是-1可以刷新数据
                this->doneCurrent();
                //std::cout << "pickRadian:" << msg.radian << std::endl;
                emit PickSectionMsg(wheel_flange_treads[this->currentWheelId].GetSection(radian));//没有东西不发数据

            }
			this->UpdateWheelFlangeTreadModel(radian);
        }
        else
        {
            std::cout << "ERROR::LOCALWIDGET::SetPickAngle::ILLEGAL Angle" << std::endl;

        }
    }
    else
    {
        std::cout << "ERROR::LOCALWIDGET::SetPickAngle::NO WHEEL TO SET PICK Angle!" << std::endl;
    }
}
void QLocalOpenGLWidget::mousePressEvent(QMouseEvent *e)
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
            //this->Pick(e->x(),e->y());
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
void QLocalOpenGLWidget::mouseReleaseEvent(QMouseEvent *e)  //在非opengl函数中调用opengAPI需要makeCurrent
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


void QLocalOpenGLWidget::mouseMoveEvent(QMouseEvent * e)
{
	if (rightButtonDown)
	{
		float xoffset = e->x() - this->lastX;
		float yoffset = this->lastY - e->y();

		//std::cout << "xoffset:" << xoffset << " yoffset:" << yoffset << endl;

		this->lastX = e->x();
		this->lastY = e->y();
		if (this->camera_move_enable)
		camera.ProcessMouseMovement(xoffset, yoffset);
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
            //this->Pick(e->x(),e->y());

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
void QLocalOpenGLWidget::Pick(int posX,int posY)
{
    if (this->currentWheelId ==0)
    {
		//std::cout << "OK1?" << std::endl;
		PickMsg msg = engine.RayPick_Wheel(posX, posY, projection, view, camera.position, this->scr_width, this->scr_height);
		//std::cout << "OK2?" << std::endl;
        if (this->pickSectionEnable&&msg.GetRadian() != -1)//防止空
        {
            this->pickMsg.SetRadian(msg.GetRadian());
            this->makeCurrent();
			shaderProgram_flange_tread.use();
			shaderProgram_flange_tread.setFloat("pickRadian", msg.GetRadian());  //是-1可以刷新数据
            this->doneCurrent();
            //std::cout << "pickRadian:" << msg.radian << std::endl;
            emit PickSectionMsg(wheel_flange_treads[this->currentWheelId].GetSection(msg.GetRadian()));//没有东西不发数据
            float angle=360.0f*msg.GetRadian() /(2* Wheel::WheelFlangeTread::kPIR);
            emit PickSectionAngle(angle);  //只有pick执行时才会发送改变gotoWidget的信号，set的时候不发
        }
   //     if (this->pickWidthEnable&&msg.GetWidth() != -1)
   //     {
			//Range range = wheels[currentWheelId].GetTreadRange();
   //         if (msg.GetWidth() >= range.GetStart() && msg.GetWidth() <= range.GetEnd())
   //         {
   //             this->pickMsg.SetWidth(msg.GetWidth());
   //             this->makeCurrent();
   //             shaderProgram.setFloat("pickWidth", msg.GetWidth());
   //             this->doneCurrent();
   //             //std::cout << "pickWidth:" << msg.width << std::endl;
   //             emit PickWidth(msg.GetWidth());
   //         }
   //     }
    }
}
//void QLocalOpenGLWidget::SetPickWidth(double width)
//{
//	if (currentWheelId != -1)
//	{
		//Range range = wheels[currentWheelId].GetTreadRange();
		//if (width >= range.GetStart() && width <= range.GetEnd())
		//{
  //          if(abs(this->pickMsg.GetWidth()-width)>0.001)//查重
  //          {
  //              this->pickMsg.SetWidth(width);
  //              this->makeCurrent();
  //              shaderProgram.setFloat("pickWidth", width);
  //              this->doneCurrent();
  //              //std::cout << "pickWidth:" << width << std::endl;
  //              emit PickWidth(width);//也要发！
  //          }
		//}
		//else
		//{
  //          std::cout << "ERROR::LOCALWIDGET::SETPICKWIDTH::ILLEGAL WIDTH! "<<range.GetStart()<<" "<<range.GetEnd() << std::endl;
		//}
//	}
//	else
//	{
//		std::cout << "ERROR::LOCALWIDGET::SETPICKWIDTH::NO WHEEL TO SET PICK WIDTH!" << std::endl;
//	}
//}

void QLocalOpenGLWidget::timerEvent(QTimerEvent *e)
{
	//if (this->keyWPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::FORWARD, 0.001*TIMESPAN);
	//if(this->keySPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::BACKWARD, 0.001*TIMESPAN);
	//if(this->keyAPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::LEFT, 0.001*TIMESPAN);
	//if (this->keyDPressEvent)
	//	camera.ProcessKeyboard(Camera_Movement::RIGHT, 0.001*TIMESPAN);
	this->update();
}

void QLocalOpenGLWidget::keyPressEvent(QKeyEvent * e)
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
void QLocalOpenGLWidget::keyReleaseEvent(QKeyEvent * e)
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

void QLocalOpenGLWidget::wheelEvent(QWheelEvent * e)
{
	if(this->camera_move_enable)
	camera.ProcessMouseScroll(e->delta());
	//this->paintGL();
}

void QLocalOpenGLWidget::Bigger()
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

void QLocalOpenGLWidget::Smaller()
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

void QLocalOpenGLWidget::InitCamera()
{
	//camera.InitCamera();
	if (this->full_screen_enable == false)
	{
		this->camera_move_enable = false;
		camera.InitCamera(kYaw, kPitch, kCameraLocalAngle, glm::vec3(0.0f, 0.0f, 0.0f), kNoFullScreenR, kZoom);
	}
	if (this->full_screen_enable == true)
	{
		this->camera_move_enable = true;
		camera.InitCamera(kYaw, kPitch, kCameraLocalAngle, glm::vec3(0.0f, 0.0f, 0.0f), kFullScreenR, kZoom);
	}
}

void QLocalOpenGLWidget::Rotate()
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
		this->camera.ProcessMouseMovement(-kRotateUnitOffset,0);
}

void QLocalOpenGLWidget::EnableSectionPick()
{
	if (this->pickSectionEnable)
	{
		this->pickSectionEnable = false;
		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("pickRadian", -1);
		this->doneCurrent();
	}
	else
	{
		this->pickSectionEnable = true;
	}
}
void QLocalOpenGLWidget::EnableWidthPick()
{
	if (this->pickWidthEnable)
	{
		this->pickWidthEnable = false;
		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("pickWidth", -1);
		this->doneCurrent();
	}
	else
	{
		this->pickWidthEnable = true;
		this->makeCurrent();
		shaderProgram_flange_tread.use();
		shaderProgram_flange_tread.setFloat("pickWidth", pickMsg.GetWidth());
		this->doneCurrent();
	}

}
//void QLocalOpenGLWidget::CoordinateAxisVisible()
//{
//    if(this->coordinateAxisVisible)
//    {
//        this->coordinateAxisVisible=false;
//    }
//    else
//    {
//        this->coordinateAxisVisible=true;
//    }
//}

void QLocalOpenGLWidget::ColorMarkVisible()
{
    if(this->colorMarkVisible)
    {
        this->colorMarkVisible =false;
    }
    else
    {
        this->colorMarkVisible =true;
    }
    this->makeCurrent();
	this->shaderProgram_flange_tread.use();
    this->shaderProgram_flange_tread.setBool("colorMark",this->colorMarkVisible);
    this->doneCurrent();
}

void QLocalOpenGLWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (this->full_screen_enable == false)
		{
			emit FullScreen(true);
			this->full_screen_enable = true;
			InitCamera();
		}
		else
		{
			/*this->setWindowFlags(Qt::SubWindow);
			this->showNormal();*/
			emit FullScreen(false);
			this->full_screen_enable = false;
			InitCamera();
		}
	}
}