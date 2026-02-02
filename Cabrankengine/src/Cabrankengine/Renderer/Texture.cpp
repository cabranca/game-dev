#include <pch.h>
#include "Texture.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalTexture.h>
#endif

#ifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLTexture.h>
#endif

#include "Renderer.h"


namespace cabrankengine::rendering {
	
	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLTexture2D>(specification);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalTexture2D>(specification);
#endif
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(const std::string& path) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLTexture2D>(path);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalTexture2D>(path);
#endif
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(const FT_Face& face) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLTexture2D>(face);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalTexture2D>(face);
#endif
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
} // namespace cabrankengine::rendering
