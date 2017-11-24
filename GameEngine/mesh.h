#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "transform.h"
#include "obj_loader.h"
#include "sceneshader.h"

class Mesh
{
public:
	Mesh(const std::string & filename);
	Mesh(const Mesh &obj);
	virtual ~Mesh();

	inline void SetDiffuseMaterial(glm::vec3 color)
	{
		m_material.diffuse_material = color;
	}

	inline void SetSpecularMaterial(glm::vec3 color)
	{
		m_material.specular_material = color;
	}

	inline void SetEmissiveMaterial(glm::vec3 color)
	{
		m_material.emissive_material = color;
	}

	inline void SetShininess(float shininess)
	{
		m_material.shininess = shininess;
	}

	inline void SetTexture(const Texture & texture)
	{
		m_texture = texture;
	}

	inline Transform & GetTransform()
	{
		return m_transform;
	}

	inline const BoundingBox & GetBoundingBox() const
	{
		BoundingBox bb;

		glm::mat4 model = m_transform.GetModel();

		bb.bb_min = glm::vec3(model * glm::vec4(m_bb.bb_min, 1));
		bb.bb_max = glm::vec3(model * glm::vec4(m_bb.bb_max, 1));

		return bb;
	}

	void Draw(void);
	void Update(Camera *cam);

private:
	enum BUFFER_TYPE { POSITION_VB = 0, TEXCOORD_VB, NORMAL_VB, INDEX_VB, NUM_BUFFERS};

	void InitMesh(const IndexedModel & model);

	std::vector<Vertex> m_vertices;

	GLuint m_vao; // Vertex Array Object
	GLuint m_vbo[NUM_BUFFERS]; // Vertex Buffer Objects
	
	uint32_t m_indices_count;

	BoundingBox m_bb;
	Texture m_texture;
	Material m_material;
	Transform m_transform;

	Shader *m_shader;
};
