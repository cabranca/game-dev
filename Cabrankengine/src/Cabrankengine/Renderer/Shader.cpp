#include <pch.h>
#include "Shader.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalShader.h>
#elifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLShader.h>
#endif

#include "Renderer.h"


namespace cabrankengine::rendering {

	Ref<Shader> Shader::create(const std::string& filepath) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLShader>(filepath);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalShader>(filepath);
#endif
		}
		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::getAPI()) {
			case RendererAPI::API::None:
				CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLShader>(name, vertexSrc, fragmentSrc);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalShader>(name, vertexSrc, fragmentSrc);
#endif
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader) {
		CE_CORE_ASSERT(!m_Shaders.contains(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader) {
		const auto& name = shader->getName();
		CE_CORE_ASSERT(!m_Shaders.contains(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filepath) {
		auto shader = Shader::create(filepath);
		add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath) {
		auto shader = Shader::create(filepath);
		add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::get(const std::string& name) {
		CE_CORE_ASSERT(m_Shaders.contains(name), "Shader not found!");
		return m_Shaders[name];
	}
}

