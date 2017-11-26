#pragma once

#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <rapidxml\rapidxml.hpp>

#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class ResourceManager 
{
public:
	~ResourceManager();

	static ResourceManager *GetInstance();

	Model *GetModel(std::string id);
	Shader *GetShader(std::string id);
	Texture *GetTexture(std::string id);

	bool Init(std::string filepath);
	Model *LoadModel(std::string id);
	Shader *LoadShader(std::string id);
	Texture *LoadTexture(std::string id);

private:
	static ResourceManager *m_instance;

	std::map<std::string, std::shared_ptr<ModelResource>> m_model_resource_map;
	std::map<std::string, std::shared_ptr<ShaderResource>> m_shader_resource_map;
	std::map<std::string, std::shared_ptr<TextureResource>> m_texture_resource_map;

	std::map<std::string, std::shared_ptr<Model>> m_model_map;
	std::map<std::string, std::shared_ptr<Shader>> m_shader_map;
	std::map<std::string, std::shared_ptr<Texture>> m_texture_map;

	ResourceManager();
};
