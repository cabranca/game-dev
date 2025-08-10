#pragma once

#include <Cabrankengine/Core/Window.h>

struct GLFWwindow; // Forward declaration of the GLFWwindow struct

namespace cabrankengine {
	
	class GraphicsContext; // Forward declaration of the GraphicsContext class

	class WindowsWindow : public Window {
		public:
			WindowsWindow(const WindowProps& props);
			virtual ~WindowsWindow();

			// Callback for the window update
			virtual void onUpdate() override;

			// Returns the window width
			virtual unsigned int getWidth() const override { return m_Data.Width; }

			// Returns the window height
			virtual unsigned int getHeight() const override { return m_Data.Height; }

			// Sets the callback function for the event polling
			virtual void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
			// Sets whether VSync is enabled or not
			virtual void setVSync(bool enabled) override;
		
			// Returns whether VSync is enabled or not
			virtual bool isVSync() const override;

			// Returns the Windows specific window.
			virtual void* getNativeWindow() const override { return m_Window; }

		private:
			// Initialize the window from the given properties
			void init(const WindowProps& props);

			// Shutsdown GLFW correctly to destroy the window
			void shutdown();

			GLFWwindow* m_Window; // Actual window object
			GraphicsContext* m_Context; // Graphics context for rendering

			// Data of the window to work with GLFW
			// TODO: could I reuse WindowProps in any way?
			struct WindowData {
				std::string Title;
				unsigned int Width, Height;
				bool VSync;

				EventCallbackFn EventCallback;
			};

			WindowData m_Data; // Current window data
	};
}
