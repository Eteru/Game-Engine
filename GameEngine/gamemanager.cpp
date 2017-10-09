
#include "gamemanager.h"

#include <iostream>
#include <GL\glew.h>

GameManager::GameManager(int width, int height, const std::string & title)
	: m_windowClosed(false)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();

	if (GLEW_OK != status) {
		std::cerr << "[GLEW]: Initialization failed" << std::endl;
	}

	// Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Cull back faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

GameManager::~GameManager()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void GameManager::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameManager::Update(void)
{
	SDL_GL_SwapWindow(m_window);

	SDL_Event e;

	while (SDL_PollEvent(&e)) {

		switch (e.type)
		{
		case SDL_KEYDOWN:
			ParseKeyPress(e.key.keysym);
			break;
		case SDL_QUIT:
			m_windowClosed = true;
			break;
		default:
			break;
		}
	}
}

bool GameManager::IsWindowClosed(void)
{
	return m_windowClosed;
}

void GameManager::ParseKeyPress(SDL_Keysym key)
{
	std::cout << "Pressed ";
	switch (key.sym)
	{
	case SDLK_a:
		std::cout << "a" << std::endl;
		break;
	case SDLK_d:
		std::cout << "d" << std::endl;
		break;
	case SDLK_w:
		std::cout << "w" << std::endl;
		break;
	case SDLK_s:
		std::cout << "s" << std::endl;
		break;
	default:
		std::cout << "nothing relevant" << std::endl;
		break;
	}
}
