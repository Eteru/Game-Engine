#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "transform.h"
#include "obj_loader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;

	Vertex(const glm::vec3 & pos, const glm::vec2 & texCoord, const glm::vec3 & normal = glm::vec3())
		: position(pos), texCoord(texCoord), normal(normal)
	{
	}
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> & vertices, std::vector<uint32_t> & indices);
	Mesh(const std::string & filename);
	Mesh(const Mesh &obj);
	virtual ~Mesh();

	inline void SetDiffuseMaterial(GLfloat color[3])
	{
		m_diffuse_material[0] = color[0];
		m_diffuse_material[1] = color[1];
		m_diffuse_material[2] = color[2];

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse_material);
	}

	inline void SetSpecularMaterial(GLfloat color[3])
	{
		m_specular_material[0] = color[0];
		m_specular_material[1] = color[1];
		m_specular_material[2] = color[2];

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular_material);
	}

	inline void SetEmissiveMaterial(GLfloat color[3])
	{
		m_emissive_material[0] = color[0];
		m_emissive_material[1] = color[1];
		m_emissive_material[2] = color[2];

		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emissive_material);
	}

	inline void SetShininess(GLfloat shininess)
	{
		m_shininess = shininess;

		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_shininess);
	}

	inline void SetTexture(const Texture & texture)
	{
		m_texture = texture;
	}

	inline Transform & GetTransform()
	{
		return m_transform;
	}

	void Draw(void);

private:
	enum BUFFER_TYPE { POSITION_VB = 0, TEXCOORD_VB, NORMAL_VB, INDEX_VB, NUM_BUFFERS};

	void InitMesh(const IndexedModel & model);

	std::vector<Vertex> m_vertices;
	Transform m_transform;

	GLuint m_vao; // Vertex Array Object
	GLuint m_vbo[NUM_BUFFERS]; // Vertex Buffer Objects
	
	uint32_t m_indices_count;

	GLfloat m_diffuse_material[3];
	GLfloat m_specular_material[3];
	GLfloat m_emissive_material[3];
	GLfloat m_shininess;

	Texture m_texture;
};
