#pragma once
#include <string>
#include <SDL/SDL.h>
#include <memory>

namespace cabrankengine
{
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	/*
	
		This class works as a wrapper for the real SDL window class

	*/
	class Window
	{
	public:
		Window();
		~Window();

		// Create the window with the given width height and flags
		int create(std::string name, int width, int height, unsigned int currentFlags);

		// Swap between buffers (previously set in CabrankEngine.cpp
		void swapBuffer();

		// Returns the window width
		int getWidth() { return m_Width; }

		// Returns the window height
		int getHeight() { return m_Height; }

	private:
		SDL_Window* m_SDLWindow;
		int m_Width, m_Height;
	};
}
