#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace Camera {
	//enum CameraDirection{ Up,Down};
	class QuaternionsCamera
	{
	private:
		static const float YAW;
		static const float PITCH;
		static const float MOUSESENSITIVTY;//鼠标灵敏度
		static const float MIDDLEMOUSESENSITIVY;
		static const float WHEELSENSITIVTY;
		static const float ZOOM;
		static const float MOVEMENTSPEED;
	public:
		//用position来确定初始角度和半径
		QuaternionsCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3 focus = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f));
		
		//参数初始化和设置
		void InitCamera();
		void InitCamera(float yaw, float pitch, float camera_angle, const glm::vec3& focus, float r, float zoom);
		void SetDisToFocus(float r);
		void SetCameraVectors(float yaw, float pitch, float camera_angle);
		void SetMouseSensitivity(float sensitivity);
		void SetMiddleMouseSensitivity(float sensitivity);
		void SetWheelSensitivity(float sensitivity);
		void SetCameraRotateAngle(float angle);
		void RotateX(float xoffset);

		glm::mat4 GetViewMatrix();

		//鼠标响应
		void ProcessMouseMovement(float xoffset, float yoffset);
		void ProcessMiddleMouseMovement(float xoffset, float yoffset);
		bool ProcessMouseScroll(float yoffset);
		//void ShowMsg()
		//{
		//	std::cout << "position: " << position.x << " " << position.y << " " << position.z << " " << std::endl;
		//	std::cout << "focus: " << focus.x << " " << focus.y << " " << focus.z << std::endl;
		//	std::cout << "up: " << up.x << " " << up.y << " " << up.z << std::endl;
		//	std::cout << "r:" << r << std::endl;
		//	std::cout << std::endl;
		//}
		// Camera Attributes
		glm::vec3 position;
		glm::vec3 focus;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;
		glm::vec3 front;

		// Camera options
		float mouseSensitivity;
		float middleMouseSensitivity;
		float wheelSensitivity;
		float zoom;
		//环绕半径
		float r;

	private:
		void UpdateCameraPosition();
	};
}
