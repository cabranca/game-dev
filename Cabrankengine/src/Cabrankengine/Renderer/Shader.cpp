#include <Cabrankengine/Renderer/Shader.h>

#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace cabrankengine {

	Shader* Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:
			CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}

