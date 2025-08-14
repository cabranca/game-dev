#include <Platform/OpenGL/OpenGLContext.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Debug/Instrumentator.h>

namespace cabrankengine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		CE_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::init() {
		CE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CE_CORE_ASSERT(status, "Failed to initialize Glad!");

		// TODO: change to static cast
		CE_CORE_INFO("OpenGL Info:");
		CE_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		CE_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		CE_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	void OpenGLContext::swapBuffers() {
		CE_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
