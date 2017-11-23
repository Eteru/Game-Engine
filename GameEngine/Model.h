#pragma once

#include <cinttypes>
#include <string>
#include "Structs.h"

class Model
{
public:
	Model();
	Model(ModelResource *mr);

	~Model();

	bool Load();

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLuint GetVBO() const
	{
		return m_vboID;
	}

	inline GLuint GetIBO(bool is_wired) const
	{
		return (true == is_wired) ? m_wiredIboID : m_iboID;
	}

	inline uint32_t GetIBOCount(bool is_wired) const
	{
		return (true == is_wired) ? m_indicesWiredCount : m_indicesCount;
	}

private:
	bool m_loaded;
	GLuint m_iboID;
	GLuint m_wiredIboID;
	GLuint m_vboID;
	uint32_t m_indicesCount;
	uint32_t m_indicesWiredCount;

	ModelResource *m_mr;
};