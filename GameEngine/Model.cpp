
#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"

#include <vector>

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

	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_iboID);
	glDeleteBuffers(1, &m_wiredIboID);
}

bool Model::Load()
{
	if (true == m_loaded) {
		return true;
	}

	if (nullptr == m_mr) {
		return false;
	}

	FILE *f =  fopen(m_mr->model_path.c_str(), "r");

	int numberOfVertices;
	fscanf(f, "NrVertices: %d\n", &numberOfVertices);
	if (numberOfVertices <= 0)
		return false;


	// Read vertices
	std::vector<Vertex> vertices(numberOfVertices);
	for (int i = 0; i < numberOfVertices; ++i) {
		fscanf(f, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%*f, %*f, %*f]; tgt:[%*f, %*f, %*f]; uv:[%f, %f];\n",
			&vertices[i].pos.x, &vertices[i].pos.y, &vertices[i].pos.z,
			&vertices[i].normal.x, &vertices[i].normal.y, &vertices[i].normal.z,
			&vertices[i].uv.x, &vertices[i].uv.y);

		vertices[i].color = Vector3(1, 1, 1);
	}

	// Generate and create vbo
	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numberOfVertices, &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Read indices
	fscanf(f, "NrIndices: %d\n", &m_indicesCount);
	if (m_indicesCount <= 0) {
		glDeleteBuffers(1, &m_vboID);
		return false;
	}

	std::vector<int> indices(m_indicesCount);
	for (int i = 0; i < m_indicesCount; i += 3) {
		fscanf(f, "   %*d.    %d,    %d,    %d\n", &indices[i], &indices[i + 1], &indices[i + 2]);
	}

	// Generate and create ibo for filled mesh
	glGenBuffers(1, &m_iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indicesCount, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create ibo buffer for wired mesh
	m_indicesWiredCount = m_indicesCount * 2;
	std::vector<int> indicesWired(m_indicesWiredCount);
	for (int i = 0, wiredCount = 0; i < m_indicesCount; i += 3, wiredCount += 6) {
		indicesWired[wiredCount] = indices[i];
		indicesWired[wiredCount + 1] = indices[i + 1];
		indicesWired[wiredCount + 2] = indices[i + 1];
		indicesWired[wiredCount + 3] = indices[i + 2];
		indicesWired[wiredCount + 4] = indices[i + 2];
		indicesWired[wiredCount + 5] = indices[i];
	}

	// Generate and create ibo for wired mesh
	glGenBuffers(1, &m_wiredIboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wiredIboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indicesWiredCount, &indicesWired[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_loaded = true;
	return true;
}
