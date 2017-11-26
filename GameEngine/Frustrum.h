#pragma once

#include "Plane.h"
#include "Structs.h"

#include <vector>

class Frustrum
{
public:
	enum ObjectLocation {INSIDE_FRUSTRUM = 0, OUTSIDE_FRUSTRUM, INTERSECTS_FRUSTRUM};
	Frustrum();
	~Frustrum();

	void SetPlanes(glm::vec3 ftl, glm::vec3 ftr, glm::vec3 fbl, glm::vec3 fbr,
		glm::vec3 ntl, glm::vec3 ntr, glm::vec3 nbl, glm::vec3 nbr);
	ObjectLocation Contains(const BoundingBox & bb) const;

private:
	enum PlaneType { TOP = 0, BOTTOM, LEFT, RIGHT, NEAR, FAR};
	static const uint16_t PLANE_COUNT = 6;
	std::vector<Plane> m_planes;
};

