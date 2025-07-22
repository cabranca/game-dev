#include "Application.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <Cabrankengine/Core/Logger.h>

namespace cabrankengine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) // TODO: check a more modern alternative.

	Application::Application() : m_Running(true)
	{
		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (true) {
			 glClearColor(1, 0, 0, 1);
			 glClear(GL_COLOR_BUFFER_BIT);
			 m_Window->onUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		CE_CORE_INFO("{0}", e.toString());
	}
}


