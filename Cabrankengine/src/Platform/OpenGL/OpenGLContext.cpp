#include <pch.h>
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cbk::platform::opengl {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		CBK_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::init() {
		CBK_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CBK_CORE_ASSERT(status, "Failed to initialize Glad!");

		CBK_CORE_INFO("OpenGL Info:");
		CBK_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		CBK_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		CBK_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	void OpenGLContext::swapBuffers() {
		CBK_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
