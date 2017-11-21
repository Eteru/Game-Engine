
#include "mesh.h"

#include <algorithm>

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

Mesh::Mesh(const Mesh & obj)
{
	m_vertices = obj.m_vertices;
	m_transform = obj.m_transform;

	m_vao = obj.m_vao;

	for (int i = 0; i < NUM_BUFFERS; ++i) {
		m_vbo[i] = obj.m_vbo[i];
	}
	
	m_indices_count = obj.m_indices_count;

	for (int i = 0; i < 3; ++i) {
		m_diffuse_material[i] = obj.m_diffuse_material[i];
		m_specular_material[i] = obj.m_specular_material[i];
		m_emissive_material[i] = obj.m_emissive_material[i];
	}

	m_shininess = obj.m_shininess;

	m_texture = obj.m_texture;
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

	// Compute bounding box
	m_bb.bb_min = glm::vec3(FLT_MAX);
	m_bb.bb_max = glm::vec3(FLT_MIN);
	for (glm::vec3 vertex : model.positions) {
		m_bb.bb_min.x = std::min(m_bb.bb_min.x, vertex.x);
		m_bb.bb_min.y = std::min(m_bb.bb_min.y, vertex.y);
		m_bb.bb_min.z = std::min(m_bb.bb_min.z, vertex.z);

		m_bb.bb_max.x = std::max(m_bb.bb_max.x, vertex.x);
		m_bb.bb_max.y = std::max(m_bb.bb_max.y, vertex.y);
		m_bb.bb_max.z = std::max(m_bb.bb_max.z, vertex.z);
	}
}
