#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;

	Vertex(glm::vec3 pos)
	{
		position = pos;
	}
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> & vertices);
	virtual ~Mesh();

	void Draw(void);

private:
	enum BUFFER_TYPE { POSITION_VB = 0, NUM_BUFFERS};
	GLuint m_vao; // Vertex Array Object
	GLuint m_vbos[NUM_BUFFERS]; // Vertex Buffer Objects
	
	uint32_t m_drawCount;
};
