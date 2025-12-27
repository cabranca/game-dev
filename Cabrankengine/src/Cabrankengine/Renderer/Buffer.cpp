#include <pch.h>
#include "Buffer.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalBuffer.h>
#elifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLBuffer.h>
#endif

#include "Renderer.h"


namespace cabrankengine::rendering {

	// TODO: check for alternatives. It seems odd to me that the base class knows the derived classes.

	Ref<VertexBuffer> VertexBuffer::create(uint32_t size) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None: 
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLVertexBuffer>(size);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalVertexBuffer>(size);
#endif
			}
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(void* vertices, uint32_t size) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None: 
			CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLVertexBuffer>(vertices, size);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalVertexBuffer>(vertices, size);
#endif
		}
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLIndexxBuffer>(indices, count);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalIndexBuffer>(indices, count);
#endif
		}
		return nullptr;
	}
}
