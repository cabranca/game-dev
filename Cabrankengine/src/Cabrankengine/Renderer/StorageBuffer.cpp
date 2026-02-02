#include <pch.h>
#include "StorageBuffer.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalStorageBuffer.h>
#endif

#ifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLStorageBuffer.h>
#endif

#include "Renderer.h"

namespace cabrankengine::rendering {

	Ref<StorageBuffer> StorageBuffer::create(uint32_t size) {
		switch (Renderer::getAPI()) {
#ifdef CE_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:
				return createRef<platform::opengl::OpenGLStorageBuffer>(size);
#endif
#ifdef CE_RENDERER_METAL
			case RendererAPI::API::Metal:
				return createRef<platform::metal::MetalStorageBuffer>(size);
#endif
		case RendererAPI::API::None:
			CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		return nullptr;
	}
} // namespace cabrankengine::rendering
