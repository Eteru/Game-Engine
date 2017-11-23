#pragma once

#include <string>
#include "Structs.h"

class Texture
{
public:
	Texture();
	Texture(TextureResource *tr);
	~Texture();

	bool Load();

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLuint GetID() const
	{
		return m_id;
	}

private:
	bool m_loaded;
	GLuint m_id;

	TextureResource *m_tr;
};