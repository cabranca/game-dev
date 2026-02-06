#include <pch.h>
#include "UniformBuffer.h"

#ifdef CE_RENDERER_OPENGL
#include <Platform/OpenGL/OpenGLUniformBuffer.h>
#endif

namespace cabrankengine::rendering {

	Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding) {
#ifdef CE_RENDERER_OPENGL
		return std::make_shared<platform::opengl::OpenGLUniformBuffer>(size, binding);
#elif defined(CE_RENDERER_METAL)
		return nullptr; // TODO: Metal UniformBuffer not yet implemented
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}
} // namespace cabrankengine::rendering