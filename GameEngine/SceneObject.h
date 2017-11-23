#pragma once

#include <vector>
#include <string>
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

class SceneObject
{
public:
	SceneObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool depth_test, std::string id);
	~SceneObject();

	void SetWired(bool is_wired);
	void SetModel(Model *model);
	void SetShader(Shader *shader);
	void AddTexture(Texture *texture);

	void Draw();

private:
	bool m_depth_test;
	bool m_is_wired;
	std::string m_id;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	Model *m_model;
	Shader *m_shader;
	std::vector<Texture *> m_textures;
};

