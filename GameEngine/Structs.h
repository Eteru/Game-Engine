#pragma once

#include <string>
#include <GL\glew.h>
#include <glm\glm.hpp>

enum ObjectType 
{
	OT_NORMAL = 0,
	OT_SKYBOX,
	OT_TERRAIN
};

enum ShaderType
{
	ST_BASIC = 0,
	ST_SCENE
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
	ShaderType type;
	std::string id;
	std::string vs_path;
	std::string fs_path;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;

	Vertex(const glm::vec3 & pos, const glm::vec2 & texCoord,
		const glm::vec3 & normal = glm::vec3())
		: position(pos), texCoord(texCoord), normal(normal)
	{
	}
};

struct Material
{
	float shininess;
	glm::vec3 diffuse_material;
	glm::vec3 specular_material;
	glm::vec3 emissive_material;
};

struct BoundingBox
{
	glm::vec3 bb_min;
	glm::vec3 bb_max;

	BoundingBox()
		: bb_min(glm::vec3(0)), bb_max(glm::vec3(0))
	{}

	BoundingBox(glm::vec3 min_p, glm::vec3 max_p)
		: bb_min(min_p), bb_max(max_p)
	{}

	inline bool Contains(const glm::vec3 & point) const
	{
		if (glm::all(glm::lessThan(bb_min, point)) &&
			glm::all(glm::greaterThan(bb_max, point))) {
			return true;
		}

		return false;
	}

	inline bool Contains(const BoundingBox & rhs_bb) const
	{
		if (glm::all(glm::lessThan(bb_min, rhs_bb.bb_min)) &&
			glm::all(glm::greaterThan(bb_max, rhs_bb.bb_max))) {
			return true;
		}
		return false;
	}
};
