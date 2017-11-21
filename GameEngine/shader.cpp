
#include "shader.h"

#include <iostream>
#include <fstream>


Shader::Shader()
{
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
