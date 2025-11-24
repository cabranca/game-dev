#include "VertexArray.h"

#include <Cabrankengine/Core/Logger.h>
#include <Platform/OpenGL/OpenGLVertexArray.h>

#include "Renderer.h"


namespace cabrankengine::rendering {

	Ref<VertexArray> VertexArray::create() {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLVertexArray>();
		}

		return nullptr;
	}
}