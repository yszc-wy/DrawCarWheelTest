#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
namespace Camera {
	class EulerAngleCamera {//欧拉角
	private:
		static const float YAW;
		static const float PITCH;
		static const float MOUSESENSITIVTY;//鼠标灵敏度
		static const float MIDDLEMOUSESENSITIVY;
		static const float WHEELSENSITIVTY;
		static const float ZOOM;
		static const float MOVEMENTSPEED;

		//static float model(glm::vec3 v)
		//{
		//	return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
		//}
	public:
		EulerAngleCamera(float r = 10.0f, glm::vec3 focus = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f));
		//EulerAngleCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 focus = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		void SetDisToFocus(float r);
		void SetFocus(const glm::vec3& focus);
		void SetAngle(float yaw, float pitch);
		void SetZoom(float zoom);

		void SetMouseSensitivity(float sensitivity);
		void SetMiddleMouseSensitivity(float sensitivity);
		void SetWheelSensitivity(float sensitivity);
		void InitCamera(float yaw, float pitch, const glm::vec3& focus, float r, float zoom);
		void RotateYaw(float yaw);
		//void InitWorldCamera();
		//void InitLocalSpreadCamera();
		//void SetFocus(glm::vec3 focus);
		glm::mat4 GetViewMatrix();
		void ProcessMouseMovement(float xoffset, float yoffset);
		void ProcessMiddleMouseMovement(float xoffset, float yoffset);
		bool ProcessMouseScroll(float yoffset);
		//void MoveCameraFocus(int value);
		void ShowMsg()
		{
			std::cout << "focus: " << focus.x << " " << focus.y << " " << focus.z << std::endl;
			std::cout << "yaw:" << yaw << " pitch:" << pitch << std::endl;
			std::cout << std::endl;
		}
		// Camera Attributes
		glm::vec3 position;
		glm::vec3 focus;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;
		glm::vec3 front;
		//
		float r;
		// Eular Angles
		float yaw;
		float pitch;
		// Camera options
		float mouseSensitivity;
		float middleMouseSensitivity;
		float wheelSensitivity;
		float zoom;

	private:
		void UpdateCameraVectors();
	};
}