#pragma once

#include <string>
#include <functional>
#include <Cabrankengine/Events/Event.h>

namespace cabrankengine {

	// Basic properties of every Window implementation
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Cabrankengine", unsigned int width = 1600, unsigned int height = 900)
			: Title(title), Width(width), Height(height) { }
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>; // Callback for the events. Here the signature is stated: it must get the event as argument and return nothing

		// TODO: Could this be pure virtual?
		virtual ~Window() {}

		// Callback for the window update
		virtual void onUpdate() = 0;

		// Returns the window width
		virtual unsigned int getWidth() const = 0;

		// Returns the window height
		virtual unsigned int getHeight() const = 0;

		// Sets the callback function for the event polling
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;

		// Sets whether VSync is enabled or not
		virtual void setVSync(bool enabled) = 0;

		// Returns whether VSync is enabled or not
		virtual bool isVSync() const = 0;

		// Returns the platform specific window implementation.
		virtual void* getNativeWindow() const = 0;

		// Only access to window creation (singleton pattern?)
		static Window* create(const WindowProps& props = WindowProps());
	};
}


