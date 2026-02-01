#include <pch.h>
#include "UniformBuffer.h"

#include <Platform/OpenGL/OpenGLUniformBuffer.h>

#include "Renderer.h"

namespace cabrankengine::rendering {

	Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding) {
		switch (Renderer::getAPI()) {
		case RendererAPI::API::OpenGL:
			return std::make_shared<platform::opengl::OpenGLUniformBuffer>(size, binding);
		case RendererAPI::API::None:
			CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		return nullptr;
	}
} // namespace cabrankengine::rendering