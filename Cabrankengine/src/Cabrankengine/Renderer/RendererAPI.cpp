#include "RendererAPI.h"

namespace cabrankengine::rendering {
    
#ifdef CE_RENDERER_OPENGL
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif

#ifdef CE_RENDERER_METAL
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::Metal;
#endif
}