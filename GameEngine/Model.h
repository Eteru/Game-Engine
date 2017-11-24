#pragma once

#include "Structs.h"
#include "obj_loader.h"

#include <cinttypes>
#include <string>

class Model
{
public:
	Model();
	Model(ModelResource *mr);

	~Model();

	bool Load();
	void Draw();


	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLuint GetVAO() const
	{
		return m_vaoId;
	}

	inline uint32_t GetIndicesCount() const
	{
		return m_indices_count;
	}

	inline const BoundingBox & GetBoundingBox() const
	{
		return m_bb;
	}

private:
	enum BUFFER_TYPE { POSITION_VB = 0, TEXCOORD_VB, NORMAL_VB, INDEX_VB, NUM_BUFFERS };

	bool m_loaded;
	GLuint m_vaoId;
	GLuint m_vbo[NUM_BUFFERS];
	uint32_t m_indices_count;

	BoundingBox m_bb;
	ModelResource *m_mr;

	void InitMesh(const IndexedModel & model);
};