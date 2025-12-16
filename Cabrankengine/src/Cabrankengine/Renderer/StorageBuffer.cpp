#include <pch.h>
#include "StorageBuffer.h"

#include <Platform/OpenGL/OpenGLStorageBuffer.h>

#include "Renderer.h"

namespace cabrankengine::rendering {

	Ref<StorageBuffer> StorageBuffer::create(uint32_t size) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::OpenGL:
			return std::make_shared<platform::opengl::OpenGLStorageBuffer>(size);
		case RendererAPI::API::None:
			CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		return nullptr;
	}
} // namespace cabrankengine::rendering
