
#include "BasicShader.h"

BasicShader::BasicShader()
	: Shader()
{
}

BasicShader::BasicShader(ShaderResource * sr)
	: Shader(sr)
{
}

BasicShader::~BasicShader()
{
}

void BasicShader::Init(const std::string & vs, const std::string & fs)
{
	m_program = glCreateProgram();

	// Load and create shaders
	m_shaders[VERTEX_SHADER] = CreateShader(LoadShader(vs), GL_VERTEX_SHADER);
	m_shaders[FRAGMENT_SHADER] = CreateShader(LoadShader(fs), GL_FRAGMENT_SHADER);

	// Attach shaders to the program
	for (size_t i = 0; i < NUM_SHADERS; ++i) {
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "i_position");

	// Link the program
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "[Program]: linking failed.");

	// Validate the program
	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "[Program]: is invallid.");

	// Vertex shader uniforms
	m_uniforms[MVP_U] = glGetUniformLocation(m_program, "u_mvp");
}

// Not implemented for this shader
void BasicShader::Update(const Transform & transform, const Camera & camera)
{
}

void BasicShader::Update(const glm::mat4 & M, const glm::mat4 & V, const glm::mat4 & P)
{
	glm::mat4 MVP = P * V * M;
	
	glUniformMatrix4fv(m_uniforms[MVP_U], 1, GL_FALSE, &MVP[0][0]);
}

