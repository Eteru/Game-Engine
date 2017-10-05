#pragma once

#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
public:
	Transform(const glm::vec3 & pos = glm::vec3(),
		const glm::vec3 & rot = glm::vec3(),
		const glm::vec3 & scale = glm::vec3(1.0f, 1.0f, 1.0f))
		: m_pos(pos), m_rot(rot), m_scale(scale)
	{
	}

	virtual ~Transform()
	{
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 P = glm::translate(m_pos);
		glm::mat4 S = glm::scale(m_scale);

		glm::mat4 Rx = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
		glm::mat4 Ry = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		glm::mat4 Rz = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));
		
		return P * (Rz * Ry * Rx) * S;
	}

	inline glm::vec3 & GetPosition(void)
	{
		return m_pos;
	}

	inline glm::vec3 & GetRotation(void)
	{
		return m_rot;
	}
	inline glm::vec3 & GetScale(void)
	{
		return m_scale;
	}

	inline void SetPosition(const glm::vec3 & pos)
	{
		m_pos = pos;
	}

	inline void SetRotation(const glm::vec3 & rot)
	{
		m_rot = rot;
	}

	inline void SetScale(const glm::vec3 & scale)
	{
		m_scale = scale;
	}

private:
	glm::vec3 m_pos;
	glm::vec3 m_rot; // TODO: quaternions
	glm::vec3 m_scale;
};

