
#include <iostream>
#include <GL\glew.h>

#include "gamemanager.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

// SDL defined main and leads to link error, must undefine it
#undef main

int main(int argc, char *args[])
{
	GameManager manager(1920, 1080, "OpenGL Window");

	Mesh mesh("./res/monkey.obj");
	Mesh mesh2("./res/monkey.obj");
	Shader shader("./res/shader");
	Texture tex("./res/bricks.jpg");
	Camera camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 70.f, manager.GetDisplayRatio(), 0.01f, 1000.f);

	float angle = 0.0f;

	manager.SetCamera(camera);
	mesh.SetTexture(tex);
	mesh2.SetTexture(tex);
	mesh2.GetTransform().GetPosition().z = 10;

	manager.AddMesh(mesh);
	manager.AddMesh(mesh2);

	manager.Run();

	return 0;
}