#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "obj_loader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;

	Vertex(const glm::vec3 & pos, const glm::vec2 & texCoord, const glm::vec3 & normal = glm::vec3())
		: position(pos), texCoord(texCoord), normal(normal)
	{
	}
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> & vertices, std::vector<uint32_t> & indices);
	Mesh(const std::string & filename);
	virtual ~Mesh();

	void Draw(void);

private:
	enum BUFFER_TYPE { POSITION_VB = 0, TEXCOORD_VB, NORMAL_VB, INDEX_VB, NUM_BUFFERS};

	void InitMesh(const IndexedModel & model);

	GLuint m_vao; // Vertex Array Object
	GLuint m_vbo[NUM_BUFFERS]; // Vertex Buffer Objects
	
	uint32_t m_drawCount;
};
