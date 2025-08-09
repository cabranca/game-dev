#pragma once

#include <Cabrankengine/Renderer/GraphicsContext.h>

// Forward declarations
struct GLFWwindow;

namespace cabrankengine {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		// Initializes the graphics context.
		virtual void init() override;

		// Buffer swapping is the process of presenting the rendered image to the screen
		// while also preparing the next frame for rendering.
		virtual void swapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle; // Handle to the GLFW window associated with this context
	};
}
