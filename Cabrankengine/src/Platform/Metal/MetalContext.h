#pragma once

#include <Cabrankengine/Renderer/GraphicsContext.h>

// Forward declarations
namespace MTL {
	class Device;
	class CommandQueue;
	class RenderPipelineState;
} // namespace MTL
namespace CA {
	class MetalLayer;
}
struct GLFWwindow;

namespace cabrankengine::platform::metal {

	class MetalContext : public rendering::GraphicsContext {
	  public:
		MetalContext(GLFWwindow* windowHandle);
		~MetalContext();

		// Initializes the graphics context.
		virtual void init() override;

		// Buffer swapping is the process of presenting the rendered image to the screen
		// while also preparing the next frame for rendering.
		virtual void swapBuffers() override;

	  private:
		GLFWwindow* m_WindowHandle; // Handle to the GLFW window associated with this context

		MTL::Device* m_Device;
		MTL::CommandQueue* m_CommandQueue;
		CA::MetalLayer* m_Swapchain;
		MTL::RenderPipelineState* m_PipelineState;
	};
} // namespace cabrankengine::platform::metal
