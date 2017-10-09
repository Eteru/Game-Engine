
#include <iostream>
#include <GL\glew.h>

#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

// SDL defined main and leads to link error, must undefine it
#undef main

int main(int argc, char *args[])
{
	Display display(800, 600, "OpenGL Window");

	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f,0.0f)),
		Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.5f,1.0f)),
		Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f,0.0f))
	};

	std::vector<uint32_t> indices = { 0, 1, 2 };

	Mesh mesh("./res/monkey.obj");
	//Mesh mesh(vertices, indices);
	Shader shader("./res/shader");
	Texture tex("./res/bricks.jpg");
	Camera camera(glm::vec3(0, 0, -3), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 70.f, display.GetDisplayRatio(), 0.01f, 1000.f);
	Transform transform;

	float angle = 0.0f;

	while (false == display.IsWindowClosed()) {
		display.Clear(0.15f, 0.15f, 0.f, 1.f);

		float sinA = sinf(angle);
		float cosA = cosf(angle);

		//transform.GetPosition().x = sinA;
		//transform.GetRotation().z = angle;
		//transform.SetScale(glm::vec3(cosA, cosA, cosA));

		shader.Bind();
		tex.Bind(0);
		shader.Update(transform, camera);
		mesh.Draw();

		display.Update();

		angle += 0.1f;
	}

	return 0;
}