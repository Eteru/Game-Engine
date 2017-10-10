#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	Camera() : m_position(glm::vec3(0, 0, -3)), m_forward(glm::vec3(0, 0, 1)), m_up(glm::vec3(0, 1, 0)), m_camera_speed(0.05f)
	{
		m_perspective = glm::perspective(70.f, 1.33f, 0.01f, 1000.f);
	}

	Camera(const glm::vec3 & pos, const glm::vec3 & forward,
		const glm::vec3 & up, float fov, float ratio, float zNear, float zFar)
		: m_position(pos), m_forward(forward), m_up(up), m_camera_speed(0.05f)
	{
		m_perspective = glm::perspective(fov, ratio, zNear, zFar);
	}

	virtual ~Camera()
	{
	}

	inline glm::vec3 GetPosition(void) const
	{
		return m_position;
	}

	inline glm::vec3 GetForward(void) const
	{
		return m_forward;
	}

	inline glm::vec3 GetUp(void) const
	{
		return m_up;
	}

	inline glm::mat4 GetViewProjection(void) const
	{
		return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	inline glm::mat4 GetViewMatrix(void) const
	{
		return glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	inline void MoveForward(void)
	{
		m_position += m_camera_speed * m_forward;
	}

	inline void MoveBackwards(void)
	{
		m_position -= m_camera_speed * m_forward;
	}

	inline void MoveLeft(void)
	{
		m_position -= glm::normalize(glm::cross(m_forward, m_up)) * m_camera_speed;
	}

	inline void MoveRight(void)
	{
		m_position += glm::normalize(glm::cross(m_forward, m_up)) * m_camera_speed;
	}

private:
	glm::mat4 m_perspective;
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;

	float m_camera_speed;
};

