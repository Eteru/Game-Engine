#include "OctreeNode.h"

OctreeNode::OctreeNode(glm::vec3 center, float r)
	: m_sq_radius(r), m_center(center)
{
	m_children.resize(CHILDREN_COUNT);
	m_children_bbs.resize(CHILDREN_COUNT);

	for (OctreeNode * node : m_children) {
		node = nullptr;
	}

	m_children_bbs[0].bb_min = center - r;
	m_children_bbs[0].bb_max = center;

	m_children_bbs[1].bb_min = glm::vec3(center.x, center.y - r, center.z - r);
	m_children_bbs[1].bb_max = glm::vec3(center.x + r, center.y, center.z);

	m_children_bbs[2].bb_min = glm::vec3(center.x, center.y - r, center.z);
	m_children_bbs[2].bb_max = glm::vec3(center.x + r, center.y, center.z + r);

	m_children_bbs[3].bb_min = glm::vec3(center.x - r, center.y - r, center.z);
	m_children_bbs[3].bb_max = glm::vec3(center.x, center.y, center.z + r);

	m_children_bbs[4].bb_min = glm::vec3(center.x - r, center.y, center.z - r);
	m_children_bbs[4].bb_max = glm::vec3(center.x, center.y + r, center.z);

	m_children_bbs[5].bb_min = glm::vec3(center.x, center.y, center.z - r);
	m_children_bbs[5].bb_max = glm::vec3(center.x + r, center.y + r, center.z);

	m_children_bbs[6].bb_min = center;
	m_children_bbs[6].bb_max = center + r;

	m_children_bbs[7].bb_min = glm::vec3(center.x - r, center.y, center.z);
	m_children_bbs[7].bb_max = glm::vec3(center.x, center.y + r, center.z + r);

	InitGL();
}

OctreeNode::~OctreeNode()
{
	for (OctreeNode * node : m_children) {
		if (nullptr != node)
			delete node;
	}


	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteVertexArrays(1, &m_vao);
}

void OctreeNode::InitGL()
{
	std::vector<glm::vec3> verts(CHILDREN_COUNT);

	verts[0] = m_center - m_sq_radius;
	verts[1] = glm::vec3(m_center.x + m_sq_radius, m_center.y - m_sq_radius, m_center.z - m_sq_radius);
	verts[2] = glm::vec3(m_center.x + m_sq_radius, m_center.y - m_sq_radius, m_center.z + m_sq_radius);
	verts[3] = glm::vec3(m_center.x - m_sq_radius, m_center.y - m_sq_radius, m_center.z + m_sq_radius);
	verts[4] = glm::vec3(m_center.x - m_sq_radius, m_center.y + m_sq_radius, m_center.z - m_sq_radius);
	verts[5] = glm::vec3(m_center.x + m_sq_radius, m_center.y + m_sq_radius, m_center.z - m_sq_radius);
	verts[6] = glm::vec3(m_center.x + m_sq_radius, m_center.y + m_sq_radius, m_center.z + m_sq_radius);
	verts[7] = glm::vec3(m_center.x - m_sq_radius, m_center.y + m_sq_radius, m_center.z + m_sq_radius);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * CHILDREN_COUNT, &verts[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint cube_elements[] = {
		// bottom
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// top
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// back
		4, 5, 1,
		1, 0, 4,
		// front
		3, 2, 6,
		6, 7, 3,
	};

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements[0]) * VERTICES_COUNT, cube_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
