#pragma once

#include <Cabrankengine/Core/Window.h>
#include <GLFW/glfw3.h> // TODO: check this inclusion. The GLFW struct could be forwarded.

namespace cabrankengine {

	// TODO: is it ok not to mark the overriden methods as virtual as well?
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		// Callback for the window update
		void onUpdate() override;

		// Returns the window width
		inline unsigned int getWidth() const override { return m_Data.Width; }

		// Returns the window height
		inline unsigned int getHeight() const override { return m_Data.Height; }

		// Sets the callback function for the event polling
		inline void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
		// Sets whether VSync is enabled or not
		void setVSync(bool enabled) override;
		
		// Returns whether VSync is enabled or not
		bool isVSync() const override;

		// Returns the Windows specific window.
		// TODO: I think it must be marked override
		inline virtual void* getNativeWindow() const { return m_Window; }

	private:
		// Initialize the window from the given properties
		virtual void init(const WindowProps& props);

		// Shutsdown GLFW correctly to destroy the window
		virtual void shutdown();

		GLFWwindow* m_Window; // Actual window object

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
