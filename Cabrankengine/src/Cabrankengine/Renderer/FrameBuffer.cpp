#include "FrameBuffer.h"

#include <Platform/OpenGL/OpenGLFrameBuffer.h>

#include "Renderer.h"

namespace cabrankengine::rendering {

    Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpecification& spec) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None: 
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLFrameBuffer>(spec);
			}
		return nullptr;
	}
}
