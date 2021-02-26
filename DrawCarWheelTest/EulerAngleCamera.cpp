#include "EulerAngleCamera.h"
#include <iostream>
//欧拉摄像机要改
//EulerAngleCamera::EulerAngleCamera(glm::vec3 position, glm::vec3 up, glm::vec3 focus, float yaw, float pitch)
//	:position(position), worldUp(up), focus(focus), yaw(yaw), pitch(pitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), mouseSensitivity(MOUSESENSITIVTY), wheelSensitivity(WHEELSENSITIVTY), Zoom(ZOOM)
//{
//	UpdateCameraVectors();
//}

namespace Camera {
	const float EulerAngleCamera::YAW = -90.0f;
	const float EulerAngleCamera::PITCH = 0.0f;
	const float EulerAngleCamera::MOUSESENSITIVTY = 0.05f;//鼠标灵敏度
	const float EulerAngleCamera::MIDDLEMOUSESENSITIVY = 0.03f;
	const float EulerAngleCamera::WHEELSENSITIVTY = 0.01f;
	const float EulerAngleCamera::ZOOM = 45.0f;
	const float EulerAngleCamera::MOVEMENTSPEED = 0.005f;
	//const float EulerAngleCamera::StartFocus = 1500.f - 15500.0f * 4.0f + 1250.0f;
	//const float EulerAngleCamera::EndFocus = 1500.0f + 15500.0f + 11250.0f;
	//const float EulerAngleCamera::FocusYMov = -465.0f - 1747.77f;
	//const float EulerAngleCamera::FocusZMov = -900.5f;
	//const float EulerAngleCamera::ValueMax = 1000.0;
	void EulerAngleCamera::SetMouseSensitivity(float sensitivity)
	{
		this->mouseSensitivity = sensitivity;
	}
	void EulerAngleCamera::SetMiddleMouseSensitivity(float sensitivity)
	{
		this->middleMouseSensitivity = sensitivity;
	}
	void EulerAngleCamera::SetWheelSensitivity(float sensitivity)
	{
		this->wheelSensitivity = sensitivity;
	}
	void EulerAngleCamera::SetDisToFocus(float r)
	{
		this->r = r;
		UpdateCameraVectors();
	}
	void EulerAngleCamera::SetFocus(const glm::vec3& focus)
	{
		this->focus = focus;
		UpdateCameraVectors();
	}
	void EulerAngleCamera::SetAngle(float yaw, float pitch)
	{
		this->yaw = yaw;
		this->pitch = pitch;
		UpdateCameraVectors();
	}
	void EulerAngleCamera::SetZoom(float zoom)
	{
		this->zoom = zoom;
		UpdateCameraVectors();
	}
	void EulerAngleCamera::InitCamera(float yaw, float pitch, const glm::vec3& focus, float r, float zoom)
	{
		this->yaw = yaw;
		this->pitch = pitch;
		this->focus = focus;
		this->r = r;
		this->zoom = zoom;
		UpdateCameraVectors();
	}
	//void EulerAngleCamera::InitWorldCamera()
	//{
	//	this->yaw = YAW;
	//	this->pitch = PITCH;
	//	this->zoom = ZOOM;
	//
	//	glm::mat4 scale(1.0f);
	//	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));
	//
	//	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(StartFocus, FocusYMov, FocusZMov));
	//
	//	glm::mat4 model = scale*translate;
	//
	//	this->focus = glm::vec3(model*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));;
	//
	//	this->r = 10.0f;
	//
	//	UpdateCameraVectors();
	//}



	//void EulerAngleCamera::InitLocalSpreadCamera()
	//{
	//	this->yaw = -90.0f;
	//	this->pitch = 0.0f;
	//	this->zoom = ZOOM;
	//	this->focus = glm::vec3(0.6f, 0.0f, 0.6f);
	//	this->r = 2.0f;
	//	this->mouseSensitivity = 0.1f;
	//	UpdateCameraVectors();
	//}

	//void EulerAngleCamera::MoveCameraFocus(int value)
	//{
	//	float ratio = value / ValueMax;
	//	float dis = EndFocus - StartFocus;
	//
	//	glm::mat4 scale(1.0f);
	//	scale = glm::scale(scale, glm::vec3(0.001f, 0.001f, 0.001f));
	//
	//	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(StartFocus + ratio*dis, FocusYMov, FocusZMov));
	//
	//	glm::mat4 model = scale*translate;
	//
	//	this->focus = glm::vec3(model*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//
	//	UpdateCameraVectors();
	//}

	EulerAngleCamera::EulerAngleCamera(float r, glm::vec3 focus, float yaw, float pitch, glm::vec3 worldUp)
		:r(r), focus(focus), yaw(yaw), pitch(pitch),
		worldUp(worldUp), mouseSensitivity(MOUSESENSITIVTY),
		wheelSensitivity(WHEELSENSITIVTY), zoom(ZOOM),
		middleMouseSensitivity(MIDDLEMOUSESENSITIVY)
	{
		UpdateCameraVectors();
	}

	void EulerAngleCamera::ProcessMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		pitch += yoffset;
		yaw += xoffset;

		/*float angle= (int)pitch % 360;*/


		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		//if (angle >= 90&&angle <270 || angle>-270&&angle <= -90)
		//{
		//	this->worldUp = glm::vec3(0.0f, -1.0f, 0.0f);
		//	yaw -= xoffset;
		//}
		//else
		//{
		//	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		//	yaw += xoffset;
		//}

		//std::cout << "yaw: pitch:" << yaw << " " << pitch << std::endl;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		//ShowMsg();
		// Update Front, Right and Up Vectors using the updated Eular angles
		UpdateCameraVectors();
	}

	void EulerAngleCamera::RotateYaw(float yaw)
	{
		this->yaw += yaw;
		UpdateCameraVectors();
	}

	void EulerAngleCamera::UpdateCameraVectors()
	{
		// Calculate the new Front vector
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);
		//
		position = focus + r*(-front);  //根据焦点，改变position
													  // Also re-calculate the Right and Up vector
		right = glm::normalize(glm::cross(front, worldUp));  //  //这样计算出来的摄像机的上向量不会与worldUp有大于90度的差距，而且一旦pitch>90度,旋转的方式与鼠标滑动方式相反
		up = glm::normalize(glm::cross(right, front));//获取up

		//std::cout <<"position:"<< position.x << " " << position.y << " " << position.z << std::endl;
	}

	glm::mat4 EulerAngleCamera::GetViewMatrix()
	{
		return glm::lookAt(position, focus, up);
	}

	bool EulerAngleCamera::ProcessMouseScroll(float yoffset)
	{
		//std::cout << "yoffset:" << yoffset << std::endl;
		//if (zoom >= 1.0f && zoom <= 90.0f)
		//	zoom -= yoffset*wheelSensitivity;
		//if (zoom <= 1.0f)
		//{
		//	zoom = 1.0f;
		//	return false;
		//}
		//if (zoom >= 90.0f)
		//{
		//	zoom = 90.0f;
		//	return false;
		//}

		float velocity = this->wheelSensitivity * yoffset;

		r -= velocity;
		if (r < 0.12)//minR
		{
			r = 0.12;
		}
		if (r > 60)
		{
			r = 60;
		}
		//std::cout << "r:" << r << std::endl;
		UpdateCameraVectors();

		return true;
	}

	void EulerAngleCamera::ProcessMiddleMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= middleMouseSensitivity;
		yoffset *= middleMouseSensitivity;
		this->focus -= xoffset*this->right + yoffset*this->up;
		UpdateCameraVectors();
		//ShowMsg();
	}

	//void EulerAngleCamera::SetFocus(glm::vec3 focus)
	//{
	//	this->focus = focus;
	//	UpdateCameraVectors();
	//}
}