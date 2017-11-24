#pragma once

#include <vector>
#include <string>
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

class SceneObject
{
public:
	SceneObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	~SceneObject();

	void SetModel(Model *model);
	void SetShader(Shader *shader);
	void AddTexture(Texture *texture);

	void Draw();


	inline Transform & GetTransform()
	{
		return m_transform;
	}


	inline const BoundingBox & GetBoundingBox() const
	{
		BoundingBox bb = m_model->GetBoundingBox();

		glm::mat4 model = m_transform.GetModel();

		bb.bb_min = glm::vec3(model * glm::vec4(bb.bb_min, 1));
		bb.bb_max = glm::vec3(model * glm::vec4(bb.bb_max, 1));

		return bb;
	}

private:
	Model *m_model;
	Shader *m_shader;
	Transform m_transform;
	std::vector<Texture *> m_textures;
};

