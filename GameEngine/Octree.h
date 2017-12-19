#pragma once

#include "OctreeNode.h"
#include "BasicShader.h"

class Octree
{
public:
	Octree(glm::vec3 wmin, glm::vec3 wmax);
	~Octree();

	void Insert(SceneObject *obj);
	void Draw(const glm::mat4 & V, const glm::mat4 & P) const;
	void DrawContainedObjects(const Frustrum & frustrum) const;
	bool CheckCameraCollision(const glm::vec3 & point) const;

private:
	GLuint m_vbo;
	OctreeNode *m_root;
	glm::vec3 m_min_point;
	glm::vec3 m_max_point;
	Shader *m_shader;
	std::vector<SceneObject *> m_objects;

	void InsertIntoNode(SceneObject *obj, OctreeNode *node);
	void DrawNodes(const OctreeNode *node, const glm::mat4 & V, const glm::mat4 & P) const;
	void DrawNodeGL(const OctreeNode *node, const glm::mat4 & M, const glm::mat4 & V, const glm::mat4 & P) const;
};

