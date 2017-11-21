#include "Octree.h"



Octree::Octree(glm::vec3 wmin, glm::vec3 wmax)
	: m_min_point(wmin), m_max_point(wmax)
{
	glm::vec3 center = wmin + (wmax - wmin) / 2.f;
	float r = (wmax.x - wmin.x) / 2.f;

	m_root = new OctreeNode(center, r);
	m_shader = new BasicShader();
	m_shader->Init("./res/basic_shader");
}

Octree::~Octree()
{
	if (nullptr != m_root)
		delete m_root;

	if (nullptr != m_shader)
		delete m_shader;
}

void Octree::Insert(Mesh * obj)
{
	InsertIntoNode(obj, m_root);
}

void Octree::Draw(const glm::mat4 & V, const glm::mat4 & P) const
{
	glLineWidth(3);
	DrawNodes(m_root, V, P);
	glLineWidth(1);
}

void Octree::InsertIntoNode(Mesh * obj, OctreeNode * node)
{
	bool is_contained = false;
	BoundingBox bb = obj->GetBoundingBox();

	for (int i = 0; i < node->m_children_bbs.size(); ++i) {
		if (true == node->m_children_bbs[i].Contains(bb)) {

			if (nullptr == node->m_children[i]) {
				glm::vec3 nmin = node->m_children_bbs[i].bb_min;
				glm::vec3 nmax = node->m_children_bbs[i].bb_max;

				glm::vec3 center = nmin + (nmax - nmin) / 2.f;
				float r = (nmax.x - nmin.x) / 2.f;

				node->m_children[i] = new OctreeNode(center, r);
			}
			
			is_contained = true;
			InsertIntoNode(obj, node->m_children[i]);
		}
	}

	if (false == is_contained) {
		node->m_objects.push_back(obj);
	}
}

void Octree::DrawNodes(const OctreeNode * node, const glm::mat4 & V, const glm::mat4 & P) const
{
	if (nullptr == node) {
		return;
	}

	glm::mat4 M = glm::translate(node->m_center) * glm::scale(glm::vec3(2 * node->m_sq_radius));

	DrawNodeGL(node, M, V, P);

	for (const OctreeNode *node : node->m_children) {
		if (nullptr != node) {
			DrawNodes(node, V, P);
		}
	}
}

void Octree::DrawNodeGL(const OctreeNode * node, const glm::mat4 & M, const glm::mat4 & V, const glm::mat4 & P) const
{
	m_shader->Bind();
	m_shader->Update(M, V, P);

	glBindVertexArray(node->m_vao);

	glDrawElements(GL_LINE_LOOP, node->VERTICES_COUNT, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}


