#include <pch.h>
#include "Buffer.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalBuffer.h>
#endif

#ifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLBuffer.h>
#endif

namespace cabrankengine::rendering {

	// TODO: check for alternatives. It seems odd to me that the base class knows the derived classes.

	Ref<VertexBuffer> VertexBuffer::create(uint32_t size) {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLVertexBuffer>(size);
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalVertexBuffer>(size);
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}

	Ref<VertexBuffer> VertexBuffer::create(void* vertices, uint32_t size) {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLVertexBuffer>(vertices, size);
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalVertexBuffer>(vertices, size);
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count) {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLIndexBuffer>(indices, count);
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalIndexBuffer>(indices, count);
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}
}
