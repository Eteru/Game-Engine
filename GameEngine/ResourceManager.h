#pragma once

#include <map>
#include "shader.h"

class ResourceManager
{
public:
	~ResourceManager();

	static ResourceManager *GetInstance();
	void AddShader(std::string filename, Shader *);
	Shader *GetShader(std::string filename);

private:
	static ResourceManager *m_instance;
	std::map<std::string, Shader*> m_shader_map;

	ResourceManager();
};

