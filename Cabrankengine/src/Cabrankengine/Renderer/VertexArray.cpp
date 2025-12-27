#include <pch.h>
#include "VertexArray.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalVertexArray.h>
#elifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLVertexArray.h>
#endif

#include "Renderer.h"


namespace cabrankengine::rendering {

	Ref<VertexArray> VertexArray::create() {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLVertexArray>();
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalVertexArray>();
#endif
		}

		return nullptr;
	}
}
