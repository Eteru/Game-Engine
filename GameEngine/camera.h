#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

class Camera
{
public:
	Camera() 
		: m_position(glm::vec3(0, 0, -3)), m_forward(glm::vec3(0, 0, 1)), m_up(glm::vec3(0, 1, 0)),
		m_camera_speed(0.005f), m_mouse_sensitivity(0.001f)
	{
		m_perspective = glm::perspective(70.f, 1.33f, 0.01f, 1000.f);
	}

	Camera(const glm::vec3 & pos, const glm::vec3 & forward,
		const glm::vec3 & up, float fov, float ratio, float zNear, float zFar)
		: m_position(pos), m_forward(forward), m_up(up), m_camera_speed(0.05f),
		m_mouse_sensitivity(0.001f)
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

	inline glm::mat4 GetProjectionMatrix(void) const
	{
		return m_perspective;
	}

	inline void MoveForward(uint32_t count)
	{
		glm::vec3 new_pos = m_position + count * m_camera_speed * m_forward;
		if (new_pos.x < 0 || new_pos.y < 0 || new_pos.z < 0) {
			return;
		}

		m_position = new_pos;
	}

	inline void MoveBackwards(uint32_t count)
	{
		glm::vec3 new_pos = m_position - count * m_camera_speed * m_forward;
		if (new_pos.x < 0 || new_pos.y < 0 || new_pos.z < 0) {
			return;
		}

		m_position = new_pos;
	}

	inline void MoveLeft(uint32_t count)
	{
		glm::vec3 new_pos = m_position - glm::normalize(glm::cross(m_forward, m_up)) * (count * m_camera_speed);
		if (new_pos.x < 0 || new_pos.y < 0 || new_pos.z < 0) {
			return;
		}

		m_position = new_pos;
	}

	inline void MoveRight(uint32_t count)
	{
		glm::vec3 new_pos = m_position + glm::normalize(glm::cross(m_forward, m_up)) * (count * m_camera_speed);
		if (new_pos.x < 0 || new_pos.y < 0 || new_pos.z < 0) {
			return;
		}

		m_position = new_pos;
	}

	inline void Rotate(int xrel, int yrel)
	{
		if (xrel != 0)
			m_forward = glm::rotate(m_forward, m_mouse_sensitivity * -xrel, glm::vec3(0.f, 1.f, 0.f));

		if (yrel != 0)
			m_forward = glm::rotate(m_forward, m_mouse_sensitivity * -yrel, glm::vec3(1.f, 0.f, 0.f));
	}

private:
	glm::mat4 m_perspective;
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;

	float m_camera_speed;
	float m_mouse_sensitivity;
};

