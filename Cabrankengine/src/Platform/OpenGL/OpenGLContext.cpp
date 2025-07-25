#include <Platform/OpenGL/OpenGLContext.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Cabrankengine/Core/Logger.h>

namespace cabrankengine {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		CE_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CE_CORE_ASSERT(status, "Failed to initialize Glad!");
	}

	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}
