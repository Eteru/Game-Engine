
#include "Octree.h"
#include "ResourceManager.h"

Octree::Octree(glm::vec3 wmin, glm::vec3 wmax)
	: m_min_point(wmin), m_max_point(wmax)
{
	glm::vec3 center = wmin + (wmax - wmin) / 2.f;
	float r = (wmax.x - wmin.x) / 2.f;

	m_root = new OctreeNode(center, r);
	m_shader = ResourceManager::GetInstance()->LoadShader("1");
}

Octree::~Octree()
{
	if (nullptr != m_root)
		delete m_root;

	if (nullptr != m_shader)
		delete m_shader;
}

void Octree::Insert(SceneObject * obj)
{
	InsertIntoNode(obj, m_root);
}

void Octree::Draw(const glm::mat4 & V, const glm::mat4 & P) const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(3);

	m_shader->Bind();
	DrawNodes(m_root, V, P);

	glLineWidth(1);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Octree::InsertIntoNode(SceneObject * obj, OctreeNode * node)
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

	glm::mat4 M = glm::mat4(1);// glm::translate(glm::mat4(1), node->m_center) * glm::scale(glm::mat4(1), glm::vec3(2 * node->m_sq_radius));

	DrawNodeGL(node, M, V, P);

	for (const OctreeNode *node : node->m_children) {
		if (nullptr != node) {
			DrawNodes(node, V, P);
		}
	}
}

void Octree::DrawNodeGL(const OctreeNode * node, const glm::mat4 & M, const glm::mat4 & V, const glm::mat4 & P) const
{
	m_shader->Update(M, V, P);

	glBindVertexArray(node->m_vao);

	glDrawElements(GL_QUADS, node->VERTICES_COUNT, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_LINE_LOOP, 0, node->CHILDREN_COUNT);

	glBindVertexArray(0);
}

void Octree::DrawContainedObjetcs(const Frustrum & frustrum) const
{
	m_root->Draw(frustrum, false);
}


