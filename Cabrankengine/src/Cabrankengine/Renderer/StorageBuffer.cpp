#include <pch.h>
#include "StorageBuffer.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalStorageBuffer.h>
#endif

#ifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLStorageBuffer.h>
#endif

namespace cabrankengine::rendering {

	Ref<StorageBuffer> StorageBuffer::create(uint32_t size) {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLStorageBuffer>(size);
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalStorageBuffer>(size);
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}
} // namespace cabrankengine::rendering
