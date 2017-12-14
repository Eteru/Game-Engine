#pragma once

#include "Plane.h"
#include "Structs.h"

#include <vector>

class Shader;

class Frustrum
{
public:
	enum ObjectLocation {INSIDE_FRUSTRUM = 0, OUTSIDE_FRUSTRUM, INTERSECTS_FRUSTRUM};
	Frustrum();
	~Frustrum();

	void SetPlanes(const glm::vec3 & ftl, const glm::vec3 & ftr, const glm::vec3 & fbl,
		const glm::vec3 & fbr, const glm::vec3 & ntl, const glm::vec3 & ntr,
		const glm::vec3 & nbl, const glm::vec3 & nbr);
	ObjectLocation Contains(const BoundingBox & bb) const;

	void Draw(const glm::mat4 & V, const glm::mat4 & P);

private:
	enum PlaneType { TOP = 0, BOTTOM, LEFT, RIGHT, NEAR, FAR};
	static const uint16_t PLANE_COUNT = 6;

	Shader *m_shader;
	GLuint m_vao, m_vbo, m_ibo;
	std::vector<Plane> m_planes;
};

