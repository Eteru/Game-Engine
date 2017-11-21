
#pragma once

#include <string>
#include <vector>
#include <SDL2\SDL.h>

#include "mesh.h"
#include "camera.h"
#include "Octree.h"
#include "sceneshader.h"

class GameManager
{
public:
	GameManager(int width, int height, const std::string & title);
	virtual ~GameManager();

	void Clear(float r, float g, float b, float a);
	void Run(void);
	void Draw(void);
	void ParseInput(void);
	bool IsWindowClosed(void);

	inline float GetDisplayRatio(void) const
	{
		int w, h;
		
		SDL_GetWindowSize(m_window, &w, &h);

		return static_cast<float>(w) / h;
	}

	inline void SetCamera(Camera & camera)
	{
		m_camera = &camera;
	}

	inline void AddMesh(Mesh & mesh)
	{
		m_meshes.push_back(&mesh);
		m_octree->Insert(&mesh);
	}

private:
	struct KeyTimePress
	{
		uint32_t up;
		uint32_t down;
		uint32_t left;
		uint32_t right;
	};

	void ParseKeyPress(SDL_Keysym key);

	void ParseKeyPressRelease();

	SDL_Window *m_window;
	SDL_GLContext m_glContext;

	bool m_windowClosed;

	Camera *m_camera;
	Shader *m_shader;
	Octree *m_octree;
	std::vector<Mesh *> m_meshes;

	KeyTimePress m_key_time_pressed;
};

