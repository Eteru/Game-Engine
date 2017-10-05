#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
public:
	Camera(const glm::vec3 & pos, const glm::vec3 & forward,
		const glm::vec3 & up, float fov, float ratio, float zNear, float zFar)
		: m_position(pos), m_forward(forward), m_up(up)
	{
		m_perspective = glm::perspective(fov, ratio, zNear, zFar);
	}

	virtual ~Camera()
	{
	}

	inline glm::vec3 & GetPosition()
	{
		return m_position;
	}

	inline glm::vec3 & GetForward()
	{
		return m_forward;
	}

	inline glm::vec3 & GetUp()
	{
		return m_up;
	}

	inline glm::mat4 GetViewProjection() const
	{
		return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
	}

private:
	glm::mat4 m_perspective;
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
};

