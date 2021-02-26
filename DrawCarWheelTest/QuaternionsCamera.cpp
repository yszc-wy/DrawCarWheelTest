#include "QuaternionsCamera.h"

namespace Camera {
	const float QuaternionsCamera::YAW = -90.0f;
	const float QuaternionsCamera::PITCH = 0.0f;
	const float QuaternionsCamera::MOUSESENSITIVTY = 0.005f;//鼠标灵敏度
	const float QuaternionsCamera::MIDDLEMOUSESENSITIVY = 0.03f;
	const float QuaternionsCamera::WHEELSENSITIVTY = 0.01f;
	const float QuaternionsCamera::ZOOM = 45.0f;
	const float QuaternionsCamera::MOVEMENTSPEED = 0.005f;
	float model(glm::vec3 v)
	{
		return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
	}
	QuaternionsCamera::QuaternionsCamera(glm::vec3 position, glm::vec3 focus, glm::vec3 worldUp)
		:position(position), focus(focus), worldUp(worldUp),
		mouseSensitivity(MOUSESENSITIVTY),
		wheelSensitivity(WHEELSENSITIVTY),
		middleMouseSensitivity(MIDDLEMOUSESENSITIVY),
		zoom(ZOOM)
	{
		r = model(position);
		front = glm::normalize(focus - position);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
	void QuaternionsCamera::InitCamera()
	{
		this->zoom = ZOOM;
		this->focus = glm::vec3(0.0f, 0.0f, 0.0f);
		this->position = glm::vec3(0.0f, 0.0f, 15.0f);
		this->r = 15.0f;

		front = glm::normalize(focus - position);//角度一定要重置
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));//up重置

		UpdateCameraPosition();
	}

	void QuaternionsCamera::SetCameraVectors(float yaw, float pitch, float camera_angle)//
	{
		//通过yaw,pitch确认方向
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);

		//通过r确定位置
		position = focus + r*(-front);  //根据焦点，改变position

		//确定摄像机自身方向
		this->SetCameraRotateAngle(camera_angle);
	}
	void QuaternionsCamera::InitCamera(float yaw, float pitch, float camera_angle, const glm::vec3& focus, float r, float zoom)
	{
		this->focus = focus;
		this->r = r;
		this->zoom = zoom;
		this->SetCameraVectors(yaw, pitch, camera_angle);
	}
	void QuaternionsCamera::SetMouseSensitivity(float sensitivity)
	{
		this->mouseSensitivity = sensitivity;
	}
	void QuaternionsCamera::SetMiddleMouseSensitivity(float sensitivity)
	{
		this->middleMouseSensitivity = sensitivity;
	}
	void QuaternionsCamera::SetWheelSensitivity(float sensitivity)
	{
		this->wheelSensitivity = sensitivity;
	}
	void QuaternionsCamera::SetDisToFocus(float r)
	{
		this->r = r;
		UpdateCameraPosition();
	}
	void QuaternionsCamera::SetCameraRotateAngle(float angle)
	{
		right = glm::normalize(glm::cross(front, worldUp));//初始化right

		glm::mat4 rotateMat(1.0f);
		rotateMat = glm::rotate(rotateMat,glm::radians(angle), this->front);
		right=rotateMat*glm::vec4(right.x, right.y, right.z,1.0f);//旋转right

		up = glm::normalize(glm::cross(right, front));//获取up
	}

	glm::mat4 QuaternionsCamera::GetViewMatrix()
	{
		return glm::lookAt(position, focus, up);
	}
	void QuaternionsCamera::ProcessMouseMovement(float xoffset, float yoffset)  //改变front和up
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		glm::quat hRotate = glm::angleAxis(-xoffset, up);
		glm::quat vRotate = glm::angleAxis(yoffset, right);

		glm::quat temp = glm::normalize(glm::cross(hRotate, vRotate));//顺序问题？
		glm::vec3 lastUp = up;
		glm::vec3 lastRight = right;
		front = glm::normalize(glm::rotate(temp, front));
		//up= glm::normalize(glm::rotate(hRotate,lastRight));
		right = glm::normalize(glm::rotate(hRotate, lastRight));
		//right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));

		//front = glm::normalize(glm::rotate(temp, front));
		UpdateCameraPosition();
	}
	void QuaternionsCamera::RotateX(float xoffset)
	{
		this->ProcessMouseMovement(xoffset,0);
	}
	void QuaternionsCamera::ProcessMiddleMouseMovement(float xoffset, float yoffset)
	{
		xoffset *= middleMouseSensitivity;
		yoffset *= middleMouseSensitivity;

		this->focus -= xoffset*this->right + yoffset*this->up;
		UpdateCameraPosition();

	}
	bool QuaternionsCamera::ProcessMouseScroll(float yoffset)
	{
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
		float velocity = MOVEMENTSPEED * yoffset;
		r -= velocity;
		if (r < 5.0f)//minR
		{
			r = 5.0f;
		}
		if (r > 20.0f)//minR
		{
			r = 20.0f;
		}
		UpdateCameraPosition();

		return true;
	}
	void QuaternionsCamera::UpdateCameraPosition()//依靠front确定position
	{
		position = focus + r*(-front);
	}
}