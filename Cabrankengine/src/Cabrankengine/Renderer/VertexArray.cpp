#include <pch.h>
#include "VertexArray.h"

#ifdef CBK_RENDERER_METAL
	#include <Platform/Metal/MetalVertexArray.h>
#endif

#ifdef CBK_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLVertexArray.h>
#endif

namespace cbk::rendering {

	Ref<VertexArray> VertexArray::create() {
#ifdef CBK_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLVertexArray>();
#elif defined(CBK_RENDERER_METAL)
		return createRef<platform::metal::MetalVertexArray>();
#else
		CBK_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}
}
