#include <pch.h>
#include "MetalContext.h"

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <Metal/Metal.hpp>

namespace cabrankengine::platform::metal {

	MetalContext::MetalContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
		CE_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void MetalContext::init() {
		CE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);

		MTL::Device* pDevice = MTL::CreateSystemDefaultDevice();

		pDevice->release();
	}

	void MetalContext::swapBuffers() {
		CE_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
