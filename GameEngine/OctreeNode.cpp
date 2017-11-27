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
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint cube_elements[] = {
		0,1,2,3,
		4,5,6,7,
		0,1,5,4,
		1,2,6,5,
		2,3,7,6,
		3,0,4,7,
	};

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), &cube_elements[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void OctreeNode::DrawObjects()
{
	for (SceneObject *obj : m_objects) {
		obj->Draw();
	}
}

void OctreeNode::Draw(const Frustrum & frustrum, bool draw)
{
	if (true == draw)
		DrawObjects();

	for (int i = 0; i < CHILDREN_COUNT; ++i) {
		if (nullptr == m_children[i]) {
			continue;
		}

		if (true == draw) {
			m_children[i]->Draw(frustrum, true);
		}
		else {
			Frustrum::ObjectLocation type = frustrum.Contains(m_children_bbs[i]);

			switch (type)
			{
			case Frustrum::INSIDE_FRUSTRUM:
				m_children[i]->Draw(frustrum, true);
				break;
			case Frustrum::INTERSECTS_FRUSTRUM:
				m_children[i]->Draw(frustrum, true);
				break;
			case Frustrum::OUTSIDE_FRUSTRUM:
			default:
				return;
			}
		}
	}
}
