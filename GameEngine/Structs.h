#pragma once

#include <string>
#include <GL\glew.h>

enum ObjectType {
	OT_NORMAL = 0,
	OT_SKYBOX,
	OT_TERRAIN
};

struct ModelResource
{
	std::string id;
	std::string model_path;
};

struct TextureResource
{
	GLenum type;
	GLenum min_filter;
	GLenum mag_filter;
	GLenum wrap_s;
	GLenum wrap_t;
	std::string id;
	std::string texture_path;
};

struct ShaderResource
{
	std::string id;
	std::string vs_path;
	std::string fs_path;
};
