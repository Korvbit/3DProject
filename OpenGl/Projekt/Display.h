#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <sdl2\SDL.h>

using namespace std;

class Display
{
public:
	Display(int width, int height, const string title);

	void SwapBuffers();
	bool IsWindowClosed();

	virtual ~Display();
private:
	SDL_Window* windowPtr;
	SDL_GLContext context;
	bool isWindowClosed;
};

#endif //DISPLAY_H