#pragma once

#include <Cabrankengine/Renderer/GraphicsContext.h>

// Forward declarations
struct GLFWwindow;

namespace cabrankengine {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}
