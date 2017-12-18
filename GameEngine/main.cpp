
#include <iostream>
#include <GL\glew.h>

#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "gamemanager.h"

// SDL defined main and leads to link error, must undefine it
#undef main

int main(int argc, char *args[])
{
	GameManager manager(1920, 1080, "OpenGL Window");

	manager.Run();

	return 0;
}