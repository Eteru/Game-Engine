#pragma once

#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture();
	Texture(const std::string & filename);
	Texture(const Texture &obj);
	virtual ~Texture();

	void Bind(uint32_t unit);

private:
	GLuint m_texture;
};

