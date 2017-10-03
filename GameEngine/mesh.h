#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;

	Vertex(glm::vec3 pos, glm::vec2 texPos)
		: position(pos), texCoord(texPos)
	{
	}
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> & vertices);
	virtual ~Mesh();

	void Draw(void);

private:
	enum BUFFER_TYPE { POSITION_VB = 0, TEXCOORD_VB, NUM_BUFFERS};
	GLuint m_vao; // Vertex Array Object
	GLuint m_vbo; // Vertex Buffer Objects
	
	uint32_t m_drawCount;
};
