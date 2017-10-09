
#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> & vertices, std::vector<uint32_t> & indices)
{
	IndexedModel model;

	for (size_t i = 0; i < vertices.size(); ++i) {
		model.positions.push_back(vertices[i].position);
		model.texCoords.push_back(vertices[i].texCoord);
		model.normals.push_back(vertices[i].normal);
	}

	for (size_t i = 0; i < indices.size(); ++i)
		model.indices.push_back(indices[i]);

	InitMesh(model);
}

Mesh::Mesh(const std::string & filename)
{
	IndexedModel model = OBJModel(filename).ToIndexedModel();

	InitMesh(model);
}

Mesh::~Mesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Mesh::Draw(void)
{
	m_texture.Bind(0);

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, m_indices_count, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::InitMesh(const IndexedModel & model)
{
	m_indices_count = model.indices.size();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(NUM_BUFFERS, m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_VB);
	glVertexAttribPointer(POSITION_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEXCOORD_VB);
	glVertexAttribPointer(TEXCOORD_VB, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_VB);
	glVertexAttribPointer(NORMAL_VB, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
