#include <pch.h>
#include "Buffer.h"

#ifdef CBK_RENDERER_METAL
	#include <Platform/Metal/MetalBuffer.h>
#endif

#ifdef CBK_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLBuffer.h>
#endif

namespace cbk::rendering {

	// TODO: check for alternatives. It seems odd to me that the base class knows the derived classes.

	Ref<VertexBuffer> VertexBuffer::create(uint32_t size) {
#ifdef CBK_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLVertexBuffer>(size);
#elif defined(CBK_RENDERER_METAL)
		return createRef<platform::metal::MetalVertexBuffer>(size);
#else
		CBK_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}

	Ref<VertexBuffer> VertexBuffer::create(void* vertices, uint32_t size) {
#ifdef CBK_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLVertexBuffer>(vertices, size);
#elif defined(CBK_RENDERER_METAL)
		return createRef<platform::metal::MetalVertexBuffer>(vertices, size);
#else
		CBK_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count) {
#ifdef CBK_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLIndexBuffer>(indices, count);
#elif defined(CBK_RENDERER_METAL)
		return createRef<platform::metal::MetalIndexBuffer>(indices, count);
#else
		CBK_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}
}
