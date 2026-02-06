#include <pch.h>
#include "VertexArray.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalVertexArray.h>
#endif

#ifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLVertexArray.h>
#endif

namespace cabrankengine::rendering {

	Ref<VertexArray> VertexArray::create() {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLVertexArray>();
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalVertexArray>();
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}
}
