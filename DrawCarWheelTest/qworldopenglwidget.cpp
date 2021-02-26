#include "qworldopenglwidget.hpp"

#include <qevent.h>
#include <qpainter.h>

#include "LoadModel.h"
#include "FilePath.h"
#include "MeshObject.h"


const float QWorldOpenglWidget::kStartFocus = 0.0f/*1500.f - 15500.0f * 4.0f + 1250.0f*/;
const float QWorldOpenglWidget::kInitFocus = 1230.0f;

//const float QWorldOpenglWidget::kEndFocus = 1500.0f + 15500.0f + 11250.0f;
const float QWorldOpenglWidget::kFocusYMov = 1000.0f/*-465.0f - 1747.77f*/;
const float QWorldOpenglWidget::kFocusZMov = 0.0f/*-900.5f*/;

const float QWorldOpenglWidget::kValueMax = 1000.0;
const float QWorldOpenglWidget::kYaw = 27.0f;
const float QWorldOpenglWidget::kRotateUnitYaw = 30.0f;
const float QWorldOpenglWidget::kPitch = -8.0f;
const float QWorldOpenglWidget::kZoom = 45.0f;
const float QWorldOpenglWidget::kR = 10.0f;
const float QWorldOpenglWidget::kRightButtonPressMouseMoveSensitivity = 0.05f;//鼠标灵敏度
const float QWorldOpenglWidget::kLeftButtonPressMouseMoveSensitivity = 0.01f;//鼠标左键移动
const float QWorldOpenglWidget::kMiddleMouseSensitivity = 0.03f;
const float QWorldOpenglWidget::kWheelSensitivity = 0.005f;
QWorldOpenglWidget::QWorldOpenglWidget(QWidget * parent) 
	: QOpenGLWidget(parent),
	leftButtonDown(false),
	rightButtonDown(false),
	rotateAction(false),
	middleButtonDown(false),
	//bigEnable(true),
	//smallerEnable(true),
	trainBoxVisible(true),
	trainFrameVisible(true),
	marshalling(3),
	keyWPressEvent(false),
	keySPressEvent(false),
	keyAPressEvent(false),
	keyDPressEvent(false)
{
	this->scr_width = this->size().width();
	this->scr_height = this->size().height();

	this->timer.start(TIMESPAN, this);
}

QWorldOpenglWidget::~QWorldOpenglWidget() {
	this->makeCurrent();

	this->doneCurrent();
}

void QWorldOpenglWidget::initializeGL()
{
	initializeOpenGLFunctions();//glad

	glViewport(0, 0, scr_width, scr_height);

	// Enable depth buffer


	//glPolygonMode(GL_FRONT_AND_BACK, GL_NORMALIZE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Enable back face culling
	//glEnable(GL_CULL_FACE);

	this->InitShaders();

	this->InitModel();

	this->InitTrain2D();

	//this->camera.ShowMsg();
	this->InitCamera();
	//this->MoveCameraFocus(0);
	//this->camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
}
void QWorldOpenglWidget::InitTrain2D()
{
	this->train_2d.set_marshalling(this->marshalling);
	this->train_2d.set_lu_point(5, 10);
	this->train_2d.BuildLUPoints();
}
void QWorldOpenglWidget::SetMarshalling(int marshalling)//要大改
{
	if (marshalling < 2)
	{
		std::cout << "Error::QWorldOpenglWidget::SetMarshalling::illegal marshalling" << std::endl;
	}
	this->marshalling = marshalling;

	//update train2d
	this->InitTrain2D();
	//update train
	engine.SetTrain(this->train_wheel, this->train_frame, this->train_box, this->train_head, this->marshalling);
	//init camera
	this->InitCamera();
}

void QWorldOpenglWidget::resizeGL(int w, int h)
{
	this->scr_width = w;
	this->scr_height = h;
	glViewport(0, 0, w, h);
}
void QWorldOpenglWidget::paintGL()
{
	QPainter painter(this);

	painter.beginNativePainting();

	this->DrawOpengl();

	painter.endNativePainting();

	this->Draw2d(painter);

}
void QWorldOpenglWidget::Draw2d(QPainter& painter)
{
	this->train_2d.Draw(painter);
}
void QWorldOpenglWidget::DrawOpengl()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//设定清屏所用的颜色
										 //glClear(GL_COLOR_BUFFER_BIT);//接受一个缓冲位(Buffer Bit)来指定要清空的缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();
	//shaderProgram.setVec3("objectColor", 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
	shaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shaderProgram.setVec3("lightPos", camera.position);
	shaderProgram.setVec3("viewPos", camera.position);

	//view/projection
	view = camera.GetViewMatrix();//3d摄像机
	shaderProgram.setMat4("view", view);

	projection = glm::perspective(glm::radians(camera.zoom), ((float)this->scr_width / this->scr_height), 0.1f, 100.0f);
	shaderProgram.setMat4("projection", projection);

	//if(this->trainBoxVisible)
	//	engine.DrawTrainBox(shaderProgram);
	//if(this->trainFrameVisible)
	//	engine.DrawTrainFrame(shaderProgram);
	//engine.DrawObjects(shaderProgram);
	engine.DrawTrain(shaderProgram,this->trainBoxVisible);

	glDisable(GL_DEPTH_TEST);
}

void QWorldOpenglWidget::InitShaders()
{
    shaderProgram.initShaderProgram(FilePath::kVTrainShaderPath,
        FilePath::kFTrainShaderPath);
}
void QWorldOpenglWidget::InitModel()
{
	LoadModel loadModel;

	//Object Arrow(loadModel.loadModel("W:/CarWheel/LuxrayDamo/LuxrayDamo/3DWheelPainter/Resources/Model/Arrow.obj"));

	glm::vec3 lightSkyBlue = glm::vec3(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f);
	Object::MeshObject TrainBox(loadModel.loadModel(std::string(FilePath::kSingleTrainBoxPath)), glm::mat4(1.0f), lightSkyBlue);

	Object::MeshObject TrainHead(loadModel.loadModel(std::string(FilePath::kSingleTrainHeadPath)), glm::mat4(1.0f), lightSkyBlue);

	glm::vec3 lightSteelBlue = glm::vec3(119.0f / 255.0f, 136.0f / 255.0f, 153.0f / 255.0f);
	Object::MeshObject TrainFrame(loadModel.loadModel(std::string(FilePath::kSingleTrainFramePath)), glm::mat4(1.0f), lightSteelBlue);

	glm::vec3 lightSlateGray = glm::vec3(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f);
	Object::MeshObject TrainWheel(loadModel.loadModel(std::string(FilePath::kTrainWheelPath)), glm::mat4(1.0f), lightSlateGray);

	this->train_box = TrainBox;
	this->train_head = TrainHead;
	this->train_frame = TrainFrame;
	this->train_wheel = TrainWheel;

	engine.SetTrain(this->train_wheel, this->train_frame, this->train_box, this->train_head, this->marshalling);
}



void QWorldOpenglWidget::SetActivateWheel(WheelSide side, int id)
{
	engine.SetActivateWheel(side, id);
}
//void QWorldOpenglWidget::InitEngine()
//{
//
//	
//	//engine.SetTrainBox(TrainBox);
//	//engine.SetTrainFrame(TrainFrame);
//	//engine.AddObject(TrainBox);
//	//engine.AddObject(TrainFrame);
//	//engine.SetWheelGroup(TrainWheel);
//	//engine.AddObject(TrainWheel);
//}
void QWorldOpenglWidget::FrontMoveCameraFocus()
{
	glm::vec2 fb_id = GetFrontAndBackFrameId();
	this->MoveCameraFocusBasisOfFrame(fb_id.x);
}
glm::vec2 QWorldOpenglWidget::GetFrontAndBackFrameId()
{
	glm::vec3 focus = this->camera.focus;

	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(1000.0f, 1000.0f, 1000.0f));

	focus = glm::vec3(scale*glm::vec4(focus, 1.0f));

	float unit = (Train::kFrameDistanceBetweenBox + Train::kFrameDistanceInBox);
	float x = focus.x;

	int min_frame_id = int(x / unit) * 2;

	int front_id;
	int back_id;
	if (x - (int(x / unit)*unit) > Train::kFrameDistanceInBox + 0.5f)
	{
		//std::cout << "1" << std::endl;
		front_id = min_frame_id + 1;
		back_id = front_id + 1;
	}
	if ((x - (int(x / unit)*unit) >= Train::kFrameDistanceInBox - 0.5f) && (x - (int(x / unit)*unit) <= Train::kFrameDistanceInBox + 0.5f))
	{
		//std::cout << "2" << std::endl;
		front_id = min_frame_id;
		back_id = min_frame_id + 2;
	}
	if ((x - (int(x / unit)*unit) < Train::kFrameDistanceInBox - 0.5f) && (x - (int(x / unit)*unit) > 0.5f))
	{
		//std::cout << "3" << std::endl;
		front_id = min_frame_id;
		back_id = front_id + 1;
	}
	if ((x - (int(x / unit)*unit) <= 0.5f))
	{
		front_id = min_frame_id - 1;
		back_id = min_frame_id + 1;
	}
	//std::cout << "Front_id:" << front_id << " Back_id:" << back_id << std::endl;
	//std::cout << "x:" << x << " unit:" << unit << std::endl;
	//std::cout << "(x - (x / unit*unit): " << x - (int(x / unit)*unit) << std::endl;
	return glm::vec2(front_id, back_id);
}

void QWorldOpenglWidget::BackMoveCameraFocus()
{
	glm::vec2 fb_id = GetFrontAndBackFrameId();
	this->MoveCameraFocusBasisOfFrame(fb_id.y);
}

void QWorldOpenglWidget::MoveCameraFocusBasisOfPercent(int value)
{
	//this->camera.MoveCameraFocus(value);
	float end_focus = Train::kFrameDistanceInBox + (this->marshalling-1)*(Train::kFrameDistanceBetweenBox + Train::kFrameDistanceInBox);

	float ratio = value / kValueMax;
	float dis = end_focus - kStartFocus;

	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(kStartFocus + ratio*dis, kFocusYMov, kFocusZMov));

	glm::mat4 model = scale*translate;

	this->camera.SetFocus(model*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	std::cout << "Move::" << kStartFocus + ratio*dis << std::endl;
	//glm::translate(glm::mat4(1.0f), glm::vec3(1500.0f + 15500.0f * 2.0f - 1250.0f, -465.0f - 1747.77f, -900.5f));
	//this->camera.SetFocus(focus);
}
void QWorldOpenglWidget::PickTrain2D(int x, int y)
{
	int box_id=this->train_2d.PickBox(x, y);
	if (box_id == -1)
	{
		return;
	}
	else
	{
		this->MoveCameraFocusBasisOfBox(box_id);
	}
}
void QWorldOpenglWidget::MoveCameraFocusBasisOfFrame(int frame_id)
{
	if (frame_id >= this->marshalling * 2|| frame_id<0)
	{
		return;
	}
	int box_offset_id = frame_id / 2;
	float box_offset=box_offset_id*(Train::kFrameDistanceBetweenBox + Train::kFrameDistanceInBox);
	float frame_offset = (frame_id % 2)*Train::kFrameDistanceInBox;

	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(kStartFocus + box_offset+ frame_offset, kFocusYMov, kFocusZMov));

	glm::mat4 model = scale*translate;

	this->camera.SetFocus(model*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}
void QWorldOpenglWidget::MoveCameraFocusBasisOfBox(int box_id)
{
	if (box_id >= this->marshalling)
	{
		return;
	}
	float box_offset = box_id*(Train::kBoxDistance);

	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(kStartFocus +Train::kFrameDistanceInBox/2.0f + box_offset, kFocusYMov, kFocusZMov));

	glm::mat4 model = scale*translate;

	this->camera.SetFocus(model*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}
void QWorldOpenglWidget::MoveCameraFocusBasisOfMouseOffset(int x_offset)
{
	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));
	float end_focus = Train::kFrameDistanceInBox + (this->marshalling - 1)*(Train::kFrameDistanceBetweenBox + Train::kFrameDistanceInBox);

	float focus_move_x = kLeftButtonPressMouseMoveSensitivity*x_offset;
	float focus_min_x = glm::vec3(scale*glm::vec4(kStartFocus, kFocusYMov, kFocusZMov,1.0f)).x;
	float focus_max_x = glm::vec3(scale*glm::vec4(kStartFocus+ end_focus, kFocusYMov, kFocusZMov, 1.0f)).x;

	glm::vec3 focus = this->camera.focus;

	float yaw = this->camera.yaw;
	if (yaw<0.0f) {
		while(1) {
			yaw += 360.0f;
			if (yaw >= 0.0) break;
		}
	}
	else if (yaw >= 360.0f) {
		while (1) {
			yaw -= 360.0f;
			if (yaw<360.0f) break;
		}
	}

	if (yaw <= 180)
	{
		if ((focus.x <= focus_min_x&&focus_move_x< 0) || (focus.x >= focus_max_x&&focus_move_x > 0))
		{
			return;
		}

		focus.x += focus_move_x;
		this->camera.SetFocus(focus);
	}
	else
	{
		if ((focus.x <= focus_min_x&&focus_move_x> 0) || (focus.x >= focus_max_x&&focus_move_x < 0))
		{
			return;
		}

		focus.x -= focus_move_x;
		if (focus.x <= focus_min_x || focus.x >= focus_max_x)
		{
			return;
		}
		this->camera.SetFocus(focus);
	}
}
void QWorldOpenglWidget::mousePressEvent(QMouseEvent *e)
{
	this->setFocus();

	if (e->button() == Qt::LeftButton)
	{
		this->lastX = e->x();
		this->lastY = e->y();
		if (this->rotateAction)
		{
			setCursor(Qt::ClosedHandCursor);
			//this->lastX = e->x();
			//this->lastY = e->y();
		}
		else
		{
			int id=engine.RayPick_TrainFrame(e->x(), e->y(), projection, view, camera.position, this->scr_width, this->scr_height);
			if (id != -1)
			{
				emit PickFrameID(id);
			}
		}
		this->leftButtonDown = true;

		this->PickTrain2D(e->x(), e->y());
	}
	if (e->button() == Qt::RightButton)////右键取消
	{
		if (this->rotateAction)
		{
			this->rotateAction = false;
			emit rotateSignal(this->rotateAction);
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
void QWorldOpenglWidget::mouseReleaseEvent(QMouseEvent *e)
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
}
void QWorldOpenglWidget::mouseMoveEvent(QMouseEvent * e)
{
	float xoffset = e->x() - this->lastX;
	float yoffset = this->lastY - e->y();
	this->lastX = e->x();
	this->lastY = e->y();

	if (rightButtonDown)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	if (leftButtonDown)
	{
		if (rotateAction)
		{
			camera.ProcessMouseMovement(xoffset, yoffset);
		}
		this->MoveCameraFocusBasisOfMouseOffset(xoffset);
	}
	if (middleButtonDown)
	{
		camera.ProcessMiddleMouseMovement(xoffset, yoffset);
		//this->camera.ShowMsg();
	}
}
void QWorldOpenglWidget::timerEvent(QTimerEvent *e)
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
void QWorldOpenglWidget::wheelEvent(QWheelEvent * e)
{
	camera.ProcessMouseScroll(e->delta());
}
void QWorldOpenglWidget::Bigger()
{
	camera.ProcessMouseScroll(120);
	//if (!camera.ProcessMouseScroll(120))
	//{
	//	this->bigEnable = false;
	//	emit biggerSignal(this->bigEnable);
	//}
	//else
	//{
	//	if (!this->smallerEnable)
	//	{
	//		this->smallerEnable = true;
	//		emit smallerSignal(this->smallerEnable);
	//	}
	//}
}
void QWorldOpenglWidget::Smaller()
{
	camera.ProcessMouseScroll(-120);
	//if (!camera.ProcessMouseScroll(-120))
	//{
	//	this->smallerEnable = false;
	//	emit smallerSignal(this->smallerEnable);
	//}
	//else
	//{
	//	if (!this->bigEnable)
	//	{
	//		this->bigEnable = true;
	//		emit biggerSignal(this->bigEnable);
	//	}
	//}
}
void QWorldOpenglWidget::InitCamera()
{
	//camera.InitWorldCamera();
	//std::cout << "What?" << std::endl;

	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));

	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(kInitFocus, kFocusYMov, kFocusZMov));

	glm::mat4 model = scale*translate;

	camera.InitCamera(kYaw, kPitch, model*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), kR, kZoom);

	camera.SetMouseSensitivity(kRightButtonPressMouseMoveSensitivity);
	camera.SetMiddleMouseSensitivity(kMiddleMouseSensitivity);
	camera.SetWheelSensitivity(kWheelSensitivity);
}
void QWorldOpenglWidget::Rotate()
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

	this->camera.RotateYaw(-kRotateUnitYaw);
}


void QWorldOpenglWidget::keyPressEvent(QKeyEvent * e)
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
void QWorldOpenglWidget::keyReleaseEvent(QKeyEvent * e)
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

//void QWorldOpenglWidget::ActivateWheel(WheelSide side, int id, int outputId)//设定要激活轮子的位置,和轮子被点击时输出的ID
//{
//	this->engine.ActivateWheel(side, id, outputId);
//}
//void QWorldOpenglWidget::FreezeWheel(WheelSide side, int id)
//{
//	this->engine.FreezeWheel(side, id);
//}
//void QWorldOpenglWidget::FreezeAllWheel()
//{
//	this->engine.FreezeAllWheel();
//}

void QWorldOpenglWidget::TrainBoxVisible()
{
	if (this->trainBoxVisible)
	{
		this->trainBoxVisible = false;
	}
	else
	{
		this->trainBoxVisible = true;

	}
}
//void QWorldOpenglWidget::TrainFrameVisible()
//{
//	if (this->trainFrameVisible)
//	{
//		this->trainFrameVisible = false;
//	}
//	else
//	{
//		this->trainFrameVisible = true;
//	}
//}
