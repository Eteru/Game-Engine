#pragma once

#include <string>
#include <GL\glew.h>

class Shader
{
public:
	Shader(const std::string & filename);
	virtual ~Shader();

	void Bind(void);

private:
	enum SHADER_TYPE { VERTEX_SHADER = 0, FRAGMENT_SHADER, NUM_SHADERS };
	
	std::string LoadShader(const std::string & filename);
	GLuint CreateShader(const std::string & code, GLenum shaderType);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & err);

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
};

