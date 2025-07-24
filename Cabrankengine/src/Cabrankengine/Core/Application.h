#pragma once

#include <memory>
#include "Core.h"
#include <Cabrankengine/Events/Event.h>
#include <Cabrankengine/Core/LayerStack.h>
#include <Cabrankengine/Events/ApplicationEvent.h>


namespace cabrankengine {

	// Forward declarations
	class Window; // Base Window class

	class CABRANKENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		// Main Application running loop
		void Run();

		// Callback for the event system
		void OnEvent(Event& e);

		// Pushes a layer to the stack on top of the other layers (always under the overlays)
		void pushLayer(Layer* layer);

		// Pushes a layer to the stack on top of the other overlays
		void pushOverlay(Layer* layer);

		// Pops the top layer from the stack
		void popLayer(Layer* layer);

		// Pops the top overlay from the stack
		void popOverlay(Layer* layer);

		// Returns a reference to the Window
		inline Window& getWindow() { return *m_Window; }
	
		// Returns a reference to the app (Singleton pattern)
		inline static Application& get() { return *s_Instance; }

	private:
		// Callback for the WindowClose Event
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window; // Ptr to the app window
		bool m_Running; // Whether the app must stop or not
		LayerStack m_LayerStack; // Stack of layers to forward the events to
	
		static Application* s_Instance; // Static instance of the app (Singleton pattern)
	};

	// To be defined in client. Only way to create the app (Singleton pattern)
	// TODO: why function and not Application method?
	Application* CreateApplication();
}
