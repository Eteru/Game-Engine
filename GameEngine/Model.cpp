
#include "Model.h"

#include <vector>
#include <algorithm>

#include "transform.h"
#include "sceneshader.h"

Model::Model() : m_mr(nullptr), m_loaded(false)
{
}

Model::Model(ModelResource *mr) : m_mr(mr), m_loaded(false)
{
}

Model::~Model()
{
	if (nullptr != m_mr) {
		delete m_mr;
	}

	glDeleteBuffers(NUM_BUFFERS, m_vbo);

	glDeleteVertexArrays(1, &m_vaoId);
}

bool Model::Load()
{
	if (true == m_loaded) {
		return true;
	}

	if (nullptr == m_mr) {
		return false;
	}

	IndexedModel model = OBJModel(m_mr->model_path).ToIndexedModel();
	
	InitMesh(model);

	return true;
}

void Model::Draw()
{
	glBindVertexArray(m_vaoId);

	glDrawElements(GL_TRIANGLES, m_indices_count, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Model::InitMesh(const IndexedModel & model)
{
	m_indices_count = model.indices.size();

	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);

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