#pragma once

#include <glm\glm.hpp>

class Plane
{
public:
	Plane();
	~Plane();

	void Init(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	float Distance(glm::vec3 point) const;

private:
	float m_D;
	glm::vec3 m_normal;
};

