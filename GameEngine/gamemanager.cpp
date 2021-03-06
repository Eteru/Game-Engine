
#include "gamemanager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

#include <iterator>
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include <rapidxml\rapidxml.hpp>
#include <rapidxml\rapidxml_print.hpp>

GameManager::GameManager(int width, int height, const std::string & title)
	: m_windowClosed(false), m_lock_cursor(SDL_TRUE), m_background_color(0.15f, 0.15f, 0.f, 1.f), m_width(width), m_height(height)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	// Record mouse movement
	SDL_SetRelativeMouseMode(m_lock_cursor);

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
	m_gui = new GUI(m_window, width, height);

	GLenum status = glewInit();

	if (GLEW_OK != status) {
		std::cerr << "[GLEW]: Initialization failed" << std::endl;
	}

	// Z-buffer
	glEnable(GL_DEPTH_TEST);

	// Cull back faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	ResourceManager *rm = ResourceManager::GetInstance();
	rm->Init("./resources/xmls/resourceManager.xml");

	SceneManager *sm = SceneManager::GetInstance();
	sm->Init("./resources/xmls/sceneManager2.xml");

	m_camera = sm->GetActiveCamera();

	m_scripts = new ScriptingModule("resources.scripts.script");

	//m_gui->MessageBoxAsk("Script and message box", m_scripts->GetWelcomeMessage());
}

GameManager::~GameManager()
{
	delete m_gui;
	delete m_scripts;
	//DumpSceneToFile("scene.xml");

	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void GameManager::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameManager::Run(void)
{
	static const uint16_t TICK_INTERVAL = 15;
	uint32_t next_time = SDL_GetTicks() + TICK_INTERVAL;

	while (false == m_windowClosed) {
		ParseInput();
		Update();

		while (false == m_collision_queue.empty()) {
			glm::vec3 point = m_collision_queue.front();
			m_gui->MessageBoxAlert("Collision detected", m_scripts->GetCollisionMessage(point.x, point.y, point.z));
			m_collision_queue.pop();
		}

		Draw();

		SDL_GL_SwapWindow(m_window);
		SDL_Delay(TimeLeft(next_time));
		next_time += TICK_INTERVAL;
	}
}

void GameManager::Draw(void)
{
	glm::vec3 bkg = SceneManager::GetInstance()->GetBackgroundColor();
	Clear(bkg.r, bkg.g, bkg.b, 1.f);

	SceneManager::GetInstance()->Draw();
	m_gui->drawAll();
	glEnable(GL_DEPTH_TEST); // drawAll disables it for some reason
}

void GameManager::Update(void)
{
	SceneManager::GetInstance()->Update();

	const glm::vec3 & point = SceneManager::GetInstance()->GetActiveCamera()->GetPosition();
	if (true == SceneManager::GetInstance()->CheckPointCollision(point)) {
		m_collision_queue.push(point);
	}
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

		m_gui->onEvent(e);
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
	
	xml_node<>* window = doc.allocate_node(node_element, "window");
	std::string w = std::to_string(m_width);
	std::string h = std::to_string(m_height);

	window->append_attribute(doc.allocate_attribute("width", w.c_str()));
	window->append_attribute(doc.allocate_attribute("height", h.c_str()));
	root->append_node(window);
	
	xml_node<>* bkg = doc.allocate_node(node_element, "background");
	std::string r = std::to_string(m_background_color.r);
	std::string g = std::to_string(m_background_color.g);
	std::string b = std::to_string(m_background_color.b);
	std::string a = std::to_string(m_background_color.a);

	bkg->append_attribute(doc.allocate_attribute("r", r.c_str()));
	bkg->append_attribute(doc.allocate_attribute("g", g.c_str()));
	bkg->append_attribute(doc.allocate_attribute("b", b.c_str()));
	bkg->append_attribute(doc.allocate_attribute("a", a.c_str()));
	root->append_node(bkg);

	// Camera
	xml_node<>* cam = doc.allocate_node(node_element, "camera");
	std::string speed = std::to_string(m_camera->GetCameraSpeed());
	std::string sensitivity = std::to_string(m_camera->GetMouseSensitivity());
	std::string fov = std::to_string(m_camera->GetFOV());
	std::string zNear = std::to_string(m_camera->GetNearPlane());
	std::string zFar = std::to_string(m_camera->GetFarPlane());

	cam->append_attribute(doc.allocate_attribute("speed", speed.c_str()));
	cam->append_attribute(doc.allocate_attribute("sensitivity", sensitivity.c_str()));
	cam->append_attribute(doc.allocate_attribute("fov", fov.c_str()));
	cam->append_attribute(doc.allocate_attribute("near", zNear.c_str()));
	cam->append_attribute(doc.allocate_attribute("far", zFar.c_str()));

	glm::vec3 pos = m_camera->GetPosition();
	xml_node<>* camera_pos = doc.allocate_node(node_element, "position");
	std::string cam_pos_x = std::to_string(pos.x);
	std::string cam_pos_y = std::to_string(pos.y);
	std::string cam_pos_z = std::to_string(pos.z);

	camera_pos->append_attribute(doc.allocate_attribute("x", cam_pos_x.c_str()));
	camera_pos->append_attribute(doc.allocate_attribute("y", cam_pos_y.c_str()));
	camera_pos->append_attribute(doc.allocate_attribute("z", cam_pos_z.c_str()));

	cam->append_node(camera_pos);

	glm::vec3 forward = m_camera->GetForward();
	xml_node<>* camera_fw = doc.allocate_node(node_element, "forward");
	std::string cam_fw_x = std::to_string(forward.x);
	std::string cam_fw_y = std::to_string(forward.y);
	std::string cam_fw_z = std::to_string(forward.z);

	camera_fw->append_attribute(doc.allocate_attribute("x", cam_fw_x.c_str()));
	camera_fw->append_attribute(doc.allocate_attribute("y", cam_fw_y.c_str()));
	camera_fw->append_attribute(doc.allocate_attribute("z", cam_fw_z.c_str()));

	cam->append_node(camera_fw);

	glm::vec3 up = m_camera->GetUp();
	xml_node<>* camera_up = doc.allocate_node(node_element, "up");
	std::string cam_up_x = std::to_string(up.x);
	std::string cam_up_y = std::to_string(up.y);
	std::string cam_up_z = std::to_string(up.z);

	camera_up->append_attribute(doc.allocate_attribute("x", cam_up_x.c_str()));
	camera_up->append_attribute(doc.allocate_attribute("y", cam_up_y.c_str()));
	camera_up->append_attribute(doc.allocate_attribute("z", cam_up_z.c_str()));

	cam->append_node(camera_up);

	root->append_node(cam);

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
	case SDLK_l:
		m_lock_cursor = m_lock_cursor == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
		SDL_SetRelativeMouseMode(m_lock_cursor);
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
	if (m_key_time_pressed.left > 0) {
		m_camera->MoveLeft(m_key_time_pressed.left);
		m_key_time_pressed.left = 0;
	}

	if (m_key_time_pressed.right > 0) {
		m_camera->MoveRight(m_key_time_pressed.right);
		m_key_time_pressed.right = 0;
	}

	if (m_key_time_pressed.up > 0) {
		m_camera->MoveForward(m_key_time_pressed.up);
		m_key_time_pressed.up = 0;
	}

	if (m_key_time_pressed.down > 0) {
		m_camera->MoveBackwards(m_key_time_pressed.down);
		m_key_time_pressed.down = 0;
	}

}