
#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> & vertices)
{
	m_drawCount = static_cast<uint32_t>(vertices.size());
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create and intiialize data buffers
	glGenBuffers(1, &m_vbo);

	// Data buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Position = 0, Texture coordinates = 1;
	glEnableVertexAttribArray(POSITION_VB);
	glEnableVertexAttribArray(TEXCOORD_VB);
	glVertexAttribPointer(POSITION_VB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(TEXCOORD_VB, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(glm::vec3)));

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Mesh::Draw(void)
{
	glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, m_drawCount);

	glBindVertexArray(0);
}
