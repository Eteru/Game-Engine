#pragma once

#include <string>
#include <SDL2\SDL.h>

class Display
{
public:
	Display(int width, int height, const std::string & title);
	virtual ~Display();

	void Clear(float r, float g, float b, float a);
	void Update(void);
	bool IsWindowClosed(void);

	inline float GetDisplayRatio(void) const
	{
		int w, h;
		
		SDL_GetWindowSize(m_window, &w, &h);

		return static_cast<float>(w) / h;
	}

private:
	SDL_Window *m_window;
	SDL_GLContext m_glContext;

	bool m_windowClosed;
};

