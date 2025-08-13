#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>
#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Events/ApplicationEvent.h>
#include <Cabrankengine/ImGui/ImGuiLayer.h>
#include <Cabrankengine/Core/Timestep.h>
#include <GLFW/glfw3.h>
#include <Cabrankengine/Renderer/Renderer.h>

namespace cabrankengine {

	Application::Application() : m_Running(true), m_LastFrameTime(0.0f)
	{
		CE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::create());
		m_Window->setEventCallback(BIND_EVENT_FN(&Application::OnEvent, this));

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {}

	void Application::Run()
	{
		while (m_Running) {
			float time = static_cast<float>(glfwGetTime()); // This should be in Platform::getTime() or similar
			Timestep timestep = time - m_LastFrameTime; // Calculate the time since the last frame
			m_LastFrameTime = time;

			 for (Layer* layer : m_LayerStack)
				 layer->onUpdate(timestep);

			 m_ImGuiLayer->begin();
			 for (Layer* layer : m_LayerStack)
				 layer->onImGuiRender();
			 m_ImGuiLayer->end();

			 m_Window->onUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(&Application::onWindowClose, this));
		CE_CORE_TRACE("{0}", e.toString());
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->onEvent(e);
			if (e.handled())
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


