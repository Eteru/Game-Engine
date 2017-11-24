
#include "Texture.h"
#include "stb_image.h"

#include <iostream>

Texture::Texture() : m_tr(nullptr), m_loaded(false)
{
}

Texture::Texture(TextureResource * tr) : m_tr(tr), m_loaded(false)
{
}

Texture::Texture(const Texture & obj)
{
	m_loaded = obj.m_loaded;
	m_id = obj.m_id;
	m_tr = obj.m_tr;
}

Texture::~Texture()
{
}

bool Texture::Load()
{
	if (true == m_loaded) {
		return true;
	}

	if (nullptr == m_tr) {
		return false;
	}

	int width, height, compCount;
	unsigned char *data = stbi_load(m_tr->texture_path.c_str(), &width, &height, &compCount, 4);

	if (nullptr == data) {
		std::cerr << "[Texture]: failed loading (" << stbi_failure_reason() << ")"
			<< m_tr->texture_path << std::endl;
		return false;
	}

	glGenTextures(1, &m_id);
	glBindTexture(m_tr->type, m_id);

	glTexParameteri(m_tr->type, GL_TEXTURE_WRAP_S, m_tr->wrap_s);
	glTexParameteri(m_tr->type, GL_TEXTURE_WRAP_T, m_tr->wrap_t);

	glTexParameterf(m_tr->type, GL_TEXTURE_MIN_FILTER, m_tr->min_filter);
	glTexParameterf(m_tr->type, GL_TEXTURE_MAG_FILTER, m_tr->mag_filter);

	glTexImage2D(m_tr->type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	m_loaded = true;
	return true;
}

