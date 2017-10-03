#pragma once

#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture(const std::string & filename);
	virtual ~Texture();

	void Bind(uint32_t unit);

private:
	GLuint m_texture;
};

