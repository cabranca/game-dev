#include <pch.h>
#include "RenderCommand.h"

#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace cabrankengine::rendering {

	RendererAPI* RenderCommand::s_RendererAPI = new platform::opengl::OpenGLRendererAPI;
}
