
#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> & vertices)
{
	m_drawCount = static_cast<uint32_t>(vertices.size());
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Create and intiialize data buffers
	glGenBuffers(NUM_BUFFERS, m_vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Positions buffer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vbos);
	glDeleteVertexArrays(1, &m_vao);
}

void Mesh::Draw(void)
{
	glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, m_drawCount);

	glBindVertexArray(0);
}
