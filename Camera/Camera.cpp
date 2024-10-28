#include "Camera.h"

Camera::Camera() {}


Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) 
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) 
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	if (keys[GLFW_KEY_SPACE])
	{
		position += up * velocity;
	}

	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		position -= up * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update(); 
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up); // look what's in front of you
}

// needs to be called when pitch and yaw changes, not when a key is pressed to move the camera. Because does operations according to the angles (pitch and yaw).
void Camera::update()
{
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front); // one unit in that direction, we just need to know where the front is, its direction. We don't care about the magnitude.

	right = glm::normalize(glm::cross(front, worldUp)); // cross product of front and worldUp gives the right vector
	up = glm::normalize(glm::cross(right, front));

}


Camera::~Camera() 
{

}