
#include "SceneManager.h"
#include "SceneObject.h"


SceneObject::SceneObject(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, bool depth_test, std::string id)
	: m_position(pos), m_rotation(rot), m_scale(scale), m_depth_test(depth_test), m_id(id), m_is_wired(false)
{
}


SceneObject::~SceneObject()
{
}

void SceneObject::SetWired(bool is_wired)
{
	m_is_wired = is_wired;
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
	glUseProgram(m_shader->GetProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(m_is_wired));

	for (Texture *tex : m_textures) {
		glBindTexture(GL_TEXTURE_2D, tex->GetID());
	}
	glActiveTexture(GL_TEXTURE0); // TODO: wat

	Shaders objShader = m_shader->GetShaderData();

	if (objShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.positionAttribute);
		glVertexAttribPointer(objShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (objShader.colorAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.colorAttribute);
		glVertexAttribPointer(objShader.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) sizeof(glm::vec3));
	}

	if (objShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.uvAttribute);
		glVertexAttribPointer(objShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) * 3));
	}

	if (objShader.matrixUniform != -1)
	{
		Camera *cam = SceneManager::GetInstance()->GetActiveCamera();
		Matrix M = Matrix().SetScale(m_scale) *
			(Matrix().SetRotationX(m_rotation.x) *
			Matrix().SetRotationY(m_rotation.y) *
			Matrix().SetRotationZ(m_rotation.z)) *
			Matrix().SetTranslation(m_position);
		Matrix MVP = M * cam->GetView() * cam->GetProjection();
		glUniformMatrix4fv(objShader.matrixUniform, 1, GL_FALSE, reinterpret_cast<GLfloat*>(MVP.m));
	}

	glDrawElements(
		true == m_is_wired ? GL_LINES : GL_TRIANGLES,      // mode
		m_model->GetIBOCount(m_is_wired),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
