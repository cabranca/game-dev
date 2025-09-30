#include "RenderCommand.h"

#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace cabrankengine {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
