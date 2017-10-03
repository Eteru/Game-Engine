
#include <iostream>
#include <GL\glew.h>

#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"

// SDL defined main and leads to link error, must undefine it
#undef main

int main(int argc, char *args[])
{
	Display display(800, 600, "OpenGL Window");

	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(-0.5f, -0.5f, 0.0f)),
		Vertex(glm::vec3(0.0f, 0.5f, 0.0f)),
		Vertex(glm::vec3(0.5f, -0.5f, 0.0f))
	};

	Mesh mesh(vertices);
	Shader shader("./res/shader");
	Texture tex("D:\\_projects\\Game-Engine\\GameEngine\\res\\tex.jpg");

	while (false == display.IsWindowClosed()) {
		display.Clear(0.15f, 0.15f, 0.f, 1.f);

		shader.Bind();
		tex.Bind(0);
		mesh.Draw();

		display.Update();
	}

	return 0;
}