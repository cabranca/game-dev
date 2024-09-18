#include "Window.h"
#include <iostream>
#include <GL/glew.h>

using namespace cabrankengine;

Window::Window() : m_SDLWindow(nullptr), m_Width(0), m_Height(0) {}

Window::~Window()
{
	delete m_SDLWindow;
}

int cabrankengine::Window::create(std::string name, int width, int height, unsigned int currentFlags)
{
	// Check the flags for the window setting
	Uint32 flags = SDL_WINDOW_OPENGL;

	if (currentFlags & INVISIBLE)
		flags |= SDL_WINDOW_HIDDEN;

	if (currentFlags & FULLSCREEN)
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	if (currentFlags & BORDERLESS)
		flags |= SDL_WINDOW_BORDERLESS;

	// Create the actual window
	m_SDLWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, flags);
	if (m_SDLWindow == nullptr)
		std::cout << "SDL Window could not be created!" << std::endl;

	// Create the OpenGL context for thw window
	SDL_GLContext glContext = SDL_GL_CreateContext(m_SDLWindow);
	if (glContext == nullptr)
		std::cout << "SDL_GL context could not be created!" << std::endl;

	// Initialize glew to help talk to the hardware if needed
	GLenum error = glewInit();
	if (error != GLEW_OK)
		std::cout << "Could not initialize glew!" << std::endl;

	// Check OpenGL version
	std::cout << "*** OpenGL Version: " << glGetString(GL_VERSION) << " ***" << std::endl;

	// Set the background color for the clear call
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Set VSYNC
	SDL_GL_SetSwapInterval(0);

	// Enable alpha blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer()
{
	SDL_GL_SwapWindow(m_SDLWindow);
}