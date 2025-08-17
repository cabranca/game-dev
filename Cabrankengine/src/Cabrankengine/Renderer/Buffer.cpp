#include <Cabrankengine/Renderer/Buffer.h>

#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLBuffer.h>

namespace cabrankengine {

	// TODO: check for alternatives. It seems odd to me that the base class knows the derived classes.

	Ref<VertexBuffer> VertexBuffer::create(uint32_t size) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None: 
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;
			case RendererAPI::API::OpenGL:
				return createRef<OpenGLVertexBuffer>(size);
			}
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None: 
			CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			return nullptr;
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLVertexBuffer>(vertices, size);
		}

		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLIndexBuffer>(indices, count);
		}

		return nullptr;
	}
}