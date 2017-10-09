
#include "shader.h"

#include <iostream>
#include <fstream>

#include <glm/gtc/matrix_inverse.hpp>

Shader::Shader(const std::string & filename)
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

Shader::~Shader()
{
	// Dettach and delete shaders
	for (size_t i = 0; i < NUM_SHADERS; ++i) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void Shader::Bind(void)
{
	glUseProgram(m_program);
}

void Shader::Update(const Transform & transform, const Camera & camera)
{
	glm::vec3 camera_pos = camera.GetPosition();
	glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(camera.GetViewMatrix()));

	glm::vec3 u_lightAmbientIntensitys = glm::vec3(0,0,0);
	glm::vec3 u_lightDiffuseIntensitys = glm::vec3(1, 0.5, 0);
	glm::vec3 u_lightSpecularIntensitys = glm::vec3(0, 1, 0);

	glm::vec3 u_matAmbientReflectances = glm::vec3(1, 1, 1);
	glm::vec3 u_matDiffuseReflectances = glm::vec3(1, 1, 1);
	glm::vec3 u_matSpecularReflectances = glm::vec3(1, 1, 1);
	float u_matShininess = 0.64f;

	glUniformMatrix4fv(m_uniforms[MODEL_U], 1, GL_FALSE, &transform.GetModel()[0][0]);
	glUniformMatrix4fv(m_uniforms[VIEW_PROJECTION_U], 1, GL_FALSE, &camera.GetViewProjection()[0][0]);
	glUniformMatrix3fv(m_uniforms[VIEW_PROJECTION_U], 1, GL_FALSE, &normal_matrix[0][0]);
	glUniform3fv(m_uniforms[LIGHT_DIR_U], 1, &camera.GetForward()[0]);
	glUniform3fv(m_uniforms[CAMERA_POSITION_U], 1, &camera_pos.x);

	glUniform3fv(m_uniforms[LIGHT_AMBIENTAL_U], 1, &u_lightAmbientIntensitys.x);
	glUniform3fv(m_uniforms[LIGHT_DIFFUSE_U], 1, &u_lightDiffuseIntensitys.x);
	glUniform3fv(m_uniforms[LIGHT_SPECULAR_U], 1, &u_lightSpecularIntensitys.x);

	glUniform3fv(m_uniforms[MATERIAL_AMBIENTAL_U], 1, &u_matAmbientReflectances.x);
	glUniform3fv(m_uniforms[MATERIAL_DIFFUSE_U], 1, &u_matDiffuseReflectances.x);
	glUniform3fv(m_uniforms[MATERIAL_SPECULAR_U], 1, &u_matSpecularReflectances.x);
	glUniform1fv(m_uniforms[MATERIAL_SHINE_U], 1, &u_matShininess);
}

std::string Shader::LoadShader(const std::string& filename)
{
	std::ifstream file;
	file.open(filename.c_str());

	std::string output;
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		std::cerr << "Unable to load shader: " << filename << std::endl;
	}
	
	return output;
}

GLuint Shader::CreateShader(const std::string & code, GLenum type)
{
	GLuint shader = glCreateShader(type);

	if (0 == shader) {
		std::cerr << "[Shader]: creation failed." << std::endl;
	}

	const GLchar *shaderCode[1];
	GLint shaderLens[1];

	// Reorganize the data for OpenGL
	shaderCode[0] = code.c_str();
	shaderLens[0] = static_cast<GLint>(code.length());

	// Load and compile the shader
	glShaderSource(shader, 1, shaderCode, shaderLens);
	glCompileShader(shader);

	// Check for compiler errors
	CheckShaderError(shader, GL_COMPILE_STATUS, false, "[Shader]: compilation failed.");

	return shader;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & err)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (GL_FALSE == success) {
		if (true == isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << err << ": '" << error << "'" << std::endl;
	}
}
