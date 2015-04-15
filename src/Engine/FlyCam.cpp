#include "FlyCam.h"

FlyCam::FlyCam(){}

FlyCam::FlyCam(float a_fov) : Camera(a_fov)
{
	m_speed = 30;
	m_up = vec3(0, 1, 0);
	m_sensitivity = 1.f;

	//default - has to be set from main
	centerX = 0;
	centerY = 0;

	m_yaw = 0;
	m_pitch = 0;
}

FlyCam::~FlyCam(){}

void FlyCam::Update(float a_dt)
{
	float mod = m_speed;

	//check for boost
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		mod *= 2;
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		mod /= 4;


	//Forward
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
	{
		m_worldTransform[3] -= m_worldTransform[2] * mod * a_dt;
	}
	//Backward
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
	{
		m_worldTransform[3] += m_worldTransform[2] * mod * a_dt;
	}

	//Left
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
	{
		m_worldTransform[3] -= m_worldTransform[0] * mod * a_dt;
	}
	//Right
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
	{
		m_worldTransform[3] += m_worldTransform[0] * mod * a_dt;
	}

	//Up
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_worldTransform[3] -= m_worldTransform[1] * mod * a_dt;
	}
	//Down
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
	{
		m_worldTransform[3] += m_worldTransform[1] * mod * a_dt;
	}

	//Rotation
	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		double x, y;

		//Get and reset the mouse position
		glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
		glfwSetCursorPos(glfwGetCurrentContext(), centerX, centerY);

		x -= centerX;
		y -= centerY;

		x /= centerX;
		y /= centerY;

		x *= -m_sensitivity;
		y *= -m_sensitivity;

		m_yaw += (float)x;
		m_pitch += (float)y;

		// restrict pitch
		if (m_pitch > glm::radians(90.f))
			m_pitch = glm::radians(90.f);
		else if (m_pitch < glm::radians(-90.f))
			m_pitch = glm::radians(-90.f);

		//Do the rotating
		mat4 p_mat = glm::rotate(m_pitch, vec3(1, 0, 0));
		mat4 y_mat = glm::rotate(m_yaw, vec3(0, 1, 0));

		mat4 transform = y_mat * p_mat;

		transform[3] = m_worldTransform[3];
		m_worldTransform = transform;

	}

	m_viewTransform = glm::inverse(m_worldTransform);
	updateProjectionViewTransform();
}