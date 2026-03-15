#include <pch.h>
#include "Texture.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalTexture.h>
#endif

#ifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLTexture.h>
#endif

namespace cabrankengine::rendering {

	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification) {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLTexture2D>(specification);
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalTexture2D>(specification);
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}

	Ref<Texture2D> Texture2D::create(const std::string& path) {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLTexture2D>(path);
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalTexture2D>(path);
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}

	Ref<Texture2D> Texture2D::create(const FT_Face& face) {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLTexture2D>(face);
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalTexture2D>(face);
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}
} // namespace cabrankengine::rendering
