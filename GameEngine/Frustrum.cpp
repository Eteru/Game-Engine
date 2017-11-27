
#include "Frustrum.h"
#include "ResourceManager.h"
#include <iostream>

Frustrum::Frustrum()
{
	m_planes.resize(PLANE_COUNT);

	m_shader = ResourceManager::GetInstance()->LoadShader("1");

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
}

Frustrum::~Frustrum()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteVertexArrays(1, &m_vao);
}

void Frustrum::SetPlanes(glm::vec3 ftl, glm::vec3 ftr, glm::vec3 fbl, glm::vec3 fbr, glm::vec3 ntl, glm::vec3 ntr, glm::vec3 nbl, glm::vec3 nbr)
{
	m_planes[TOP].Init(ntr, ntl, ftl);
	m_planes[BOTTOM].Init(nbl, nbr, fbr);
	m_planes[LEFT].Init(ntl, nbl, fbl);
	m_planes[RIGHT].Init(nbr, ntr, fbr);
	m_planes[NEAR].Init(ntr, ntl, nbl);
	m_planes[FAR].Init(ftr, ftl, fbl);

	std::vector<glm::vec3> verts = 	{ftl, ftr, fbr, fbl, ntl, ntr, nbr, nbl};
	
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint cube_elements[] = {
		4,5,6,7, // near
		0,1,2,3, // far
		0,1,5,4, // top
		1,2,6,5, // right
		2,3,7,6, // bottom
		3,0,4,7, // left
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), &cube_elements[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

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

void Frustrum::Draw(const glm::mat4 & V, const glm::mat4 & P)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	m_shader->Bind();
	m_shader->Update(glm::scale(glm::vec3(.5f)), V, P);

	glBindVertexArray(m_vao);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_LINE_LOOP, 0, node->CHILDREN_COUNT);

	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
