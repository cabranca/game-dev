#include "Application.h"
#include "Window.h"
#include <glad/glad.h> // TODO: check this inclusion. Due to the preprocessor definition, including glfw failed so I replaced it with glad.
#include <Cabrankengine/Core/Logger.h>

namespace cabrankengine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) // TODO: check a more modern alternative.

	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Running(true)
	{
		CE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running) {
			 glClearColor(1, 0, 0, 1);
			 glClear(GL_COLOR_BUFFER_BIT);

			 for (Layer* layer : m_LayerStack)
				 layer->onUpdate();

			 m_Window->onUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		CE_CORE_TRACE("{0}", e.toString());
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::pushLayer(Layer* layer) {
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer) {
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::popLayer(Layer* layer) {
		m_LayerStack.popLayer(layer);
	}

	void Application::popOverlay(Layer* layer) {
		m_LayerStack.popOverlay(layer);
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}


