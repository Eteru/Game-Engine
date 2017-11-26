#include "Plane.h"



Plane::Plane()
{
}


Plane::~Plane()
{
}

void Plane::Init(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 aux1, aux2;

	aux1 = p1 - p2;
	aux2 = p3 - p2;

	m_normal = glm::normalize(glm::cross(aux2, aux1));

	float inner = m_normal.x * p2.x + m_normal.y * p2.y + m_normal.z * p2.z;
	m_D = -inner;
}

float Plane::Distance(glm::vec3 point) const
{
	float inner = m_normal.x * point.x + m_normal.y * point.y + m_normal.z * point.z;
	return m_D + inner;
}
