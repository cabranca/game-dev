#include <pch.h>
#include "FrameBuffer.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalFrameBuffer.h>
#elifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLFrameBuffer.h>
#endif

#include "Renderer.h"

namespace cabrankengine::rendering {

    Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpecification& spec) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None: 
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLFrameBuffer>(spec);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalFrameBuffer>(spec);
#endif
		}
		return nullptr;
	}
}
