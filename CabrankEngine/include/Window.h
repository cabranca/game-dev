#pragma once
#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>

namespace cabrankengine
{
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	/*
	
		This class works as a wrapper for the real SDL window class

	*/
	class Window
	{
	public:

		// Create the window with the given width height and flags
		int create(std::string name, int width, int height, unsigned int currentFlags);

		// Swap between buffers (previously set in CabrankEngine.cpp
		void swapBuffer();

	private:
		SDL_Window* m_SDLWindow;
	};
}
