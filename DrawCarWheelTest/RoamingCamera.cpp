#include "RoamingCamera.h"
#include <iostream>

const float RoamingCamera::YAW = 90.0f;
const float RoamingCamera::PITCH = 0.0f;
const float RoamingCamera::SPEED = 2.5f;
const float RoamingCamera::SENSITIVTY = 0.05f;//���������
const float RoamingCamera::ZOOM = 45.0f;

RoamingCamera::RoamingCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
{
	position = position;
	worldUp = up;
	yaw = yaw;
	pitch = pitch;
	updateCameraVectors();
}

RoamingCamera::RoamingCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
{
	position = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	yaw = yaw;
	pitch = pitch;
	updateCameraVectors();
}

glm::mat4 RoamingCamera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void RoamingCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
	//std::cout << "position:" << position.x << " " << position.y << " " << position.z << std::endl;
}

void RoamingCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//std::cout << "Yaw:" << yaw << std::endl;
	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

void RoamingCamera::ProcessMouseScroll(float yoffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

void RoamingCamera::updateCameraVectors()
{
	// Calculate the new Front vector
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

void RoamingCamera::setPosition(glm::vec3 position)
{
	this->position = position;
	updateCameraVectors();
}