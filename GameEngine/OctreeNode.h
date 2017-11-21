#pragma once

#include "mesh.h"

struct OctreeNode
{
	static const short CHILDREN_COUNT = 8;
	static const short VERTICES_COUNT = 36;

	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_vao;

	float m_sq_radius;
	glm::vec3 m_center;

	std::vector<Mesh *> m_objects;
	std::vector<OctreeNode *> m_children;
	std::vector<BoundingBox> m_children_bbs;

	OctreeNode(glm::vec3 center, float r);
	~OctreeNode();

private:
	void InitGL();
};

