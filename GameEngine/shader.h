#pragma once

#include <string>
#include <GL\glew.h>

#include "camera.h"
#include "transform.h"
#include "Structs.h"

class Shader
{
public:
	Shader();
	Shader(ShaderResource *sr);
	virtual ~Shader();

	bool Load();

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLint GetProgramID() const
	{
		return m_program;
	}
	

	void Bind(void);

	virtual void Init(const std::string & vs, const std::string & fs) = 0;
	virtual void Update(const Transform & transform, const Camera & camera) = 0;
	virtual void Update(const glm::mat4 & M, const glm::mat4 & V, const glm::mat4 & P) = 0;

protected:
	enum SHADER_TYPE { VERTEX_SHADER = 0, FRAGMENT_SHADER, NUM_SHADERS };
	enum UNIFORMS 
	{
		MODEL_U = 0,
		VIEW_PROJECTION_U,
		MVP_U,
		NORMAL_MATRIX_U,
		LIGHT_DIR_U,
		CAMERA_POSITION_U,
		LIGHT_AMBIENTAL_U,
		LIGHT_DIFFUSE_U,
		LIGHT_SPECULAR_U,
		MATERIAL_AMBIENTAL_U,
		MATERIAL_DIFFUSE_U,
		MATERIAL_SPECULAR_U,
		MATERIAL_SHINE_U,
		NUM_UNIFORMS
	};

	bool m_loaded;
	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];

	ShaderResource *m_sr;
	
	std::string LoadShader(const std::string & filename);
	GLuint CreateShader(const std::string & code, GLenum shaderType);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & err);
};

