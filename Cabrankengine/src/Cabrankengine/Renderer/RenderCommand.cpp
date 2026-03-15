#include <pch.h>
#include "RenderCommand.h"

#ifdef CE_RENDERER_OPENGL
#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace cabrankengine::rendering {

	RendererAPI* RenderCommand::s_RendererAPI = new platform::opengl::OpenGLRendererAPI;
}
#endif

#ifdef CE_RENDERER_METAL
#include <Platform/Metal/MetalRendererAPI.h>

namespace cabrankengine::rendering {

	RendererAPI* RenderCommand::s_RendererAPI = new platform::metal::MetalRendererAPI;
}
#endif
