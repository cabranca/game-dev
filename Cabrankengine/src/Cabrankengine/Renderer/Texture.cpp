#include <Cabrankengine/Renderer/Texture.h>

#include <Cabrankengine/Renderer/Renderer.h>
#include <Cabrankengine/Core/Logger.h>	
#include <Platform/OpenGL/OpenGLTexture.h>

namespace cabrankengine {
	
	Texture2D* Texture2D::create(const TextureSpecification& specification)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLTexture2D(specification);
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Texture2D* Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return  new OpenGLTexture2D(path);
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
