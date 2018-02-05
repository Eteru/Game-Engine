#pragma once

#include <map>
#include <string>
#include <iostream>
#include <rapidxml\rapidxml.hpp>

#include "SceneObject.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Octree.h"

class SceneManager
{
public:
	~SceneManager();

	static SceneManager *GetInstance();

	bool Init(std::string filepath);
	void Update();
	void Draw();
	bool CheckPointCollision(const glm::vec3 & point);

	inline Camera *GetActiveCamera()
	{
		return m_cameras[m_active_camera].get();
	}

	inline glm::vec3 GetBackgroundColor() const
	{
		return m_background_color;
	}

private:
	static SceneManager *m_instance;

	Octree *m_octree;
	glm::vec3 m_background_color;
	std::map<std::string, std::shared_ptr<Camera>> m_cameras;
	std::map<std::string, std::shared_ptr<SceneObject>> m_objects;
	std::string m_active_camera;

	SceneManager();
};

