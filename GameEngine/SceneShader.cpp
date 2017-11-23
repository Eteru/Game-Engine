
#include "SceneShader.h"

#include <glm/gtc/matrix_inverse.hpp>

SceneShader::SceneShader()
	: Shader()
{
}

SceneShader::~SceneShader()
{
}

void SceneShader::Init(const std::string & filename)
{
	m_program = glCreateProgram();

	// Load and create shaders
	m_shaders[VERTEX_SHADER] = CreateShader(LoadShader(filename + ".vs"), GL_VERTEX_SHADER);
	m_shaders[FRAGMENT_SHADER] = CreateShader(LoadShader(filename + ".fs"), GL_FRAGMENT_SHADER);

	// Attach shaders to the program
	for (size_t i = 0; i < NUM_SHADERS; ++i) {
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "i_position");
	glBindAttribLocation(m_program, 1, "i_texCoords");
	glBindAttribLocation(m_program, 2, "i_normal");

	// Link the program
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "[Program]: linking failed.");

	// Validate the program
	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "[Program]: is invallid.");

	// Vertex shader uniforms
	m_uniforms[MODEL_U] = glGetUniformLocation(m_program, "u_model");
	m_uniforms[VIEW_PROJECTION_U] = glGetUniformLocation(m_program, "u_viewProjection");
	m_uniforms[NORMAL_MATRIX_U] = glGetUniformLocation(m_program, "u_normalMatrix");
	m_uniforms[LIGHT_DIR_U] = glGetUniformLocation(m_program, "u_lightPosition");
	m_uniforms[CAMERA_POSITION_U] = glGetUniformLocation(m_program, "u_cameraPosition");

	// Fragment shader uniforms
	// Light colors
	m_uniforms[LIGHT_AMBIENTAL_U] = glGetUniformLocation(m_program, "u_lightA");
	m_uniforms[LIGHT_DIFFUSE_U] = glGetUniformLocation(m_program, "u_lightD");
	m_uniforms[LIGHT_SPECULAR_U] = glGetUniformLocation(m_program, "u_lightS");

	// Materials colors
	m_uniforms[MATERIAL_AMBIENTAL_U] = glGetUniformLocation(m_program, "u_materialA");
	m_uniforms[MATERIAL_DIFFUSE_U] = glGetUniformLocation(m_program, "u_materialD");
	m_uniforms[MATERIAL_SPECULAR_U] = glGetUniformLocation(m_program, "u_materialS");
	m_uniforms[MATERIAL_SHINE_U] = glGetUniformLocation(m_program, "u_materialShine");
}

void SceneShader::Update(const Transform & transform, const Camera & camera)
{
	glm::vec3 camera_pos = camera.GetPosition();
	glm::vec3 light_dir = camera_pos + camera.GetForward();
	glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(camera.GetViewMatrix()));

	glm::vec3 u_lightAmbientIntensitys = glm::vec3(0.6, 0.3, 0);
	glm::vec3 u_lightDiffuseIntensitys = glm::vec3(1, 0.5, 0);
	glm::vec3 u_lightSpecularIntensitys = glm::vec3(1, 1, 0);

	glm::vec3 u_matAmbientReflectances = glm::vec3(1, 1, 1);
	glm::vec3 u_matDiffuseReflectances = glm::vec3(1, 1, 1);
	glm::vec3 u_matSpecularReflectances = glm::vec3(1, 1, 1);
	float u_matShininess = 64.f;

	glUniformMatrix4fv(m_uniforms[MODEL_U], 1, GL_FALSE, &transform.GetModel()[0][0]);
	glUniformMatrix4fv(m_uniforms[VIEW_PROJECTION_U], 1, GL_FALSE, &camera.GetViewProjection()[0][0]);
	glUniformMatrix3fv(m_uniforms[NORMAL_MATRIX_U], 1, GL_FALSE, &normal_matrix[0][0]);
	glUniform3fv(m_uniforms[LIGHT_DIR_U], 1, &light_dir.x);
	glUniform3fv(m_uniforms[CAMERA_POSITION_U], 1, &camera_pos.x);

	glUniform3fv(m_uniforms[LIGHT_AMBIENTAL_U], 1, &u_lightAmbientIntensitys.x);
	glUniform3fv(m_uniforms[LIGHT_DIFFUSE_U], 1, &u_lightDiffuseIntensitys.x);
	glUniform3fv(m_uniforms[LIGHT_SPECULAR_U], 1, &u_lightSpecularIntensitys.x);

	glUniform3fv(m_uniforms[MATERIAL_AMBIENTAL_U], 1, &u_matAmbientReflectances.x);
	glUniform3fv(m_uniforms[MATERIAL_DIFFUSE_U], 1, &u_matDiffuseReflectances.x);
	glUniform3fv(m_uniforms[MATERIAL_SPECULAR_U], 1, &u_matSpecularReflectances.x);
	glUniform1fv(m_uniforms[MATERIAL_SHINE_U], 1, &u_matShininess);
}

// Not implemented for this shader
void SceneShader::Update(const glm::mat4 & M, const glm::mat4 & V, const glm::mat4 & P)
{
}
