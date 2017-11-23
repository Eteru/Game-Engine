#include "ResourceManager.h"

ResourceManager *ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
	for (auto it : m_shader_map) {
		delete it.second;
		it.second = nullptr;
	}
}

ResourceManager * ResourceManager::GetInstance()
{
	if (nullptr == m_instance) {
		m_instance = new ResourceManager;
	}

	return m_instance;
}

void ResourceManager::AddShader(std::string filename, Shader *shader)
{
	if (m_shader_map.find(filename) != m_shader_map.end()) {
		return;
	}

	m_shader_map[filename] = shader;
}

Shader *ResourceManager::GetShader(std::string filename)
{
	if (m_shader_map.find(filename) != m_shader_map.end()) {
		return m_shader_map[filename];
	}

	return nullptr;
}
