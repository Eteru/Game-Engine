
#include "SceneManager.h"
#include "SceneObject.h"


SceneObject::SceneObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	m_transform.GetPosition() = pos;
	m_transform.GetRotation() = rot;
	m_transform.GetScale() = scale;
}


SceneObject::~SceneObject()
{
}

void SceneObject::SetModel(Model * model)
{
	m_model = model;
}

void SceneObject::SetShader(Shader * shader)
{
	m_shader = shader;
}

void SceneObject::AddTexture(Texture * texture)
{
	m_textures.push_back(texture);
}

void SceneObject::Draw()
{
	m_shader->Bind();
	m_shader->Update(m_transform, *SceneManager::GetInstance()->GetActiveCamera());
	
	/*for (Texture *tex : m_textures) {
		glBindTexture(GL_TEXTURE_2D, tex->GetID());
	}*/
	glActiveTexture(GL_TEXTURE0); // TODO: wat
	glBindTexture(GL_TEXTURE_2D, m_textures[0]->GetID());

	m_model->Draw();
}
