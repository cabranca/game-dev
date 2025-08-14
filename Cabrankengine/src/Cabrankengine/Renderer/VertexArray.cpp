#include <Cabrankengine/Renderer/VertexArray.h>

#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLVertexArray.h>

namespace cabrankengine {

	Ref<VertexArray> VertexArray::create() {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return createRef<OpenGLVertexArray>();
		}

		return nullptr;
	}
}