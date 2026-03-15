#include <pch.h>
#include "FrameBuffer.h"

#ifdef CE_RENDERER_METAL
	#include <Platform/Metal/MetalFrameBuffer.h>
#endif

#ifdef CE_RENDERER_OPENGL
	#include <Platform/OpenGL/OpenGLFrameBuffer.h>
#endif

namespace cabrankengine::rendering {

	Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpecification& spec) {
#ifdef CE_RENDERER_OPENGL
		return createRef<platform::opengl::OpenGLFrameBuffer>(spec);
#elif defined(CE_RENDERER_METAL)
		return createRef<platform::metal::MetalFrameBuffer>(spec);
#else
		CE_CORE_ASSERT(false, "No renderer API defined!");
		return nullptr;
#endif
	}
}
