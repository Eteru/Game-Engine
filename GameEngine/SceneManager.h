#pragma once

#include <map>
#include <string>
#include <iostream>
#include <rapidxml\rapidxml.hpp>
#include "SceneObject.h"
#include "ResourceManager.h"
#include "Camera.h"

class SceneManager
{
public:
	~SceneManager();

	static SceneManager *GetInstance();

	bool Init(std::string filepath);
	void Update();
	void Draw();

	inline Camera *GetActiveCamera()
	{
		return m_cameras[m_active_camera];
	}

private:
	static SceneManager *m_instance;

	glm::vec3 m_background_color;
	std::map<std::string, Camera *> m_cameras;
	std::map<std::string, SceneObject *> m_objects;
	std::string m_active_camera;

	SceneManager();
};

