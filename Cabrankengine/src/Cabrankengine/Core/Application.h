#pragma once

#include <memory>
#include "Core.h"
#include <Cabrankengine/Events/Event.h>
#include <Cabrankengine/Core/LayerStack.h>
#include <Cabrankengine/Events/ApplicationEvent.h>

namespace cabrankengine {
	class Window;
	class CABRANKENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);
		void popLayer(Layer* layer);
		void popOverlay(Layer* layer);

		inline Window& getWindow() { return *m_Window; }
	
		inline static Application& Get() { return *s_Instance; }
	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running;
		LayerStack m_LayerStack;
	
		static Application* s_Instance;
	};

	// To be defined in client.
	Application* CreateApplication();
}
