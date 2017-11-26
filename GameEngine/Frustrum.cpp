
#include "Frustrum.h"
#include <iostream>

Frustrum::Frustrum()
{
	m_planes.resize(PLANE_COUNT);
}

Frustrum::~Frustrum()
{
}

void Frustrum::SetPlanes(glm::vec3 ftl, glm::vec3 ftr, glm::vec3 fbl, glm::vec3 fbr, glm::vec3 ntl, glm::vec3 ntr, glm::vec3 nbl, glm::vec3 nbr)
{
	m_planes[TOP].Init(ntr, ntl, ftl);
	m_planes[BOTTOM].Init(nbl, nbr, fbr);
	m_planes[LEFT].Init(ntl, nbl, fbl);
	m_planes[RIGHT].Init(nbr, ntr, fbr);
	m_planes[NEAR].Init(ntr, ntl, nbl);
	m_planes[FAR].Init(ftr, ftl, fbl);
}

Frustrum::ObjectLocation Frustrum::Contains(const BoundingBox & bb) const
{
	for (const Plane & p : m_planes) {
		if (p.Distance(bb.bb_max) < 0) {
			std::cerr << "Outside" << std::endl;
			return OUTSIDE_FRUSTRUM;
		}
		else if (p.Distance(bb.bb_min) < 0) {
			std::cerr << "Intersects" << std::endl;
			return INTERSECTS_FRUSTRUM;
		}
	}

	std::cerr << "Inside" << std::endl;

	return INSIDE_FRUSTRUM;
}
