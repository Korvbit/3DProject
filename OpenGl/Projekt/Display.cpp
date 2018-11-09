#include "Display.h"
#include <glew/glew.h>
#include <iostream>

Display::Display(int width, int height, const string title)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	windowPtr = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(windowPtr);

	GLenum status = glewInit();

	if (status != GLEW_OK) 
	{
		std::cerr << "Glew failed to initialize!" << endl;
	}

	isWindowClosed = false;
}

void Display::SwapBuffers()
{
	SDL_GL_SwapWindow(windowPtr);

	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			isWindowClosed = true;
	}
}

bool Display::IsWindowClosed()
{
	return isWindowClosed;
}

Display::~Display()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(windowPtr);
	SDL_Quit();
}