
#include "shader.h"

#include <iostream>
#include <fstream>

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

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoords");
	glBindAttribLocation(m_program, 2, "normal");

	// Link the program
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "[Program]: linking failed.");

	// Validate the program
	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "[Program]: is invallid.");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
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
	glm::mat4 MVP = camera.GetViewProjection() *  transform.GetModel();

	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &MVP[0][0]);
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
