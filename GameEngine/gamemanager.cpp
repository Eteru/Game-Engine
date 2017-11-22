
#include "gamemanager.h"

#include <iterator>
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include <rapidxml\rapidxml.hpp>
#include <rapidxml\rapidxml_print.hpp>

GameManager::GameManager(int width, int height, const std::string & title)
	: m_windowClosed(false)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	// Record mouse movement
	SDL_SetRelativeMouseMode(SDL_TRUE);

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

	// Set full screen
	//SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

	m_shader = new SceneShader();
	m_shader->Init("./res/scene_shader");

	// TODO: this can be done better
	m_octree = new Octree(glm::vec3(0), glm::vec3(50));
}

GameManager::~GameManager()
{
	DumpSceneToFile("scene.xml");

	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

	delete m_shader;
}

void GameManager::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static inline uint32_t time_left(uint32_t next_time)
{
	uint32_t now = SDL_GetTicks();

	if (next_time <= now)
		return 0;
	else
		return next_time - now;
}

void GameManager::Run(void)
{
	static const uint16_t TICK_INTERVAL = 15;
	uint32_t next_time = SDL_GetTicks() + TICK_INTERVAL;

	while (false == m_windowClosed) {
		Draw();

		ParseInput();		

		SDL_GL_SwapWindow(m_window);

		SDL_Delay(time_left(next_time));
		next_time += TICK_INTERVAL;
	}
}

void GameManager::Draw(void)
{
	Clear(0.15f, 0.15f, 0.f, 1.f);

	for (Mesh *m : m_meshes) {
		m_shader->Bind();
		m_shader->Update(m->GetTransform(), *m_camera);
		m->Draw();
	}

	m_octree->Draw(m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix());
}

void GameManager::ParseInput(void)
{
	int xrel, yrel;
	SDL_Event e;

	while (SDL_PollEvent(&e)) {

		switch (e.type)
		{
		case SDL_KEYDOWN:
			ParseKeyPress(e.key.keysym);
			break;
		case SDL_MOUSEMOTION:
			SDL_GetRelativeMouseState(&xrel, &yrel);
			m_camera->Rotate(xrel, yrel);
			break;
		case SDL_QUIT:
			m_windowClosed = true;
			break;
		default:
			break;
		}
	}

	ParseKeyPressRelease();
}

bool GameManager::IsWindowClosed(void)
{
	return m_windowClosed;
}

bool GameManager::LoadSceneFromFile(const std::string & filname)
{
	return true;
}

bool GameManager::DumpSceneToFile(const std::string & filname)
{
	using namespace rapidxml;

	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	xml_node<>* root = doc.allocate_node(node_element, "scene");
	//root->append_attribute(doc.allocate_attribute("version", "1.0"));
	//root->append_attribute(doc.allocate_attribute("type", "example"));
	doc.append_node(root);
	
	std::ofstream file(filname);
	file << doc;
	file.close();
	doc.clear();

	return true;
}

void GameManager::ParseKeyPress(SDL_Keysym key)
{
	switch (key.sym)
	{
	case SDLK_a:
	case SDLK_LEFT:
		++m_key_time_pressed.left;
		break;
	case SDLK_d:
	case SDLK_RIGHT:
		++m_key_time_pressed.right;
		break;
	case SDLK_w:
	case SDLK_UP:
		++m_key_time_pressed.up;
		break;
	case SDLK_s:
	case SDLK_DOWN:
		++m_key_time_pressed.down;
		break;
	case SDLK_ESCAPE:
		m_windowClosed = true;
		break;
	default:
		break;
	}
}

void GameManager::ParseKeyPressRelease()
{
	m_camera->MoveLeft(m_key_time_pressed.left);
	m_key_time_pressed.left = 0;

	m_camera->MoveRight(m_key_time_pressed.right);
	m_key_time_pressed.right = 0;

	m_camera->MoveForward(m_key_time_pressed.up);
	m_key_time_pressed.up = 0;

	m_camera->MoveBackwards(m_key_time_pressed.down);
	m_key_time_pressed.down = 0;

}