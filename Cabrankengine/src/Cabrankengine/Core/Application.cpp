#include "Application.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace cabrankengine {

	Application::Application() : m_Running(true)
	{
		//m_Window = std::unique_ptr<Window>(Window::create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (true) {
			// glClearColor(1, 0, 0, 1);
			// glClear(GL_COLOR_BUFFER_BIT);
			// m_Window->onUpdate();
		}
	}
}


