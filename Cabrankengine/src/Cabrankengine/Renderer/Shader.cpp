#include <Cabrankengine/Renderer/Shader.h>

#include <Cabrankengine/Core/Logger.h>
#include <Cabrankengine/Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace cabrankengine {

	Ref<Shader> Shader::create(const std::string& filepath) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(filepath);
		}
		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}

