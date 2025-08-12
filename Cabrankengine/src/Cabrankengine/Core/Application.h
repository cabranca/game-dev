#pragma once

#include <memory>
#include <Cabrankengine/Core/LayerStack.h>

namespace cabrankengine {

	// Forward declarations
	class Event; // Base class for all events
	class ImGuiLayer; // Overlay for ImGui rendering
	class Window; // Base Window class
	class WindowCloseEvent; // Event triggerd when the close window button is pressed.
	

	class Application {
		public:
			Application();
			virtual ~Application() = default;

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
			Window& getWindow() { return *m_Window; }
	
			// Returns a reference to the app (Singleton-ish pattern)
			static Application& get() { return *s_Instance; }

		private:
			// Callback for the WindowClose Event
			bool onWindowClose(WindowCloseEvent& e);

			std::unique_ptr<Window> m_Window; // Ptr to the app window
			ImGuiLayer* m_ImGuiLayer; // ImGui layer for rendering the UI
			bool m_Running; // Whether the app must stop or not
			LayerStack m_LayerStack; // Stack of layers to forward the events to
			float m_LastFrameTime; // Time of the last frame
	
			inline static Application* s_Instance = nullptr; // Static instance of the app (Singleton-ish pattern)
	};

	// To be defined in client. Only way to create the app. It's similar to singleton but it allows more instances.
	// This is a function and not a method because it decouples the EntryPoint from the Application class.
	Application* createApplication();
}
