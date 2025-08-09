#pragma once

#include <Cabrankengine/Renderer/RendererAPI.h>

namespace cabrankengine {
	
	class RenderCommand {
	public:
		static void init() { s_RendererAPI->init(); }

		static void setClearColor(const glm::vec4& color) { s_RendererAPI->setClearColor(color); }

		static void clear() { s_RendererAPI->clear(); }

		static void drawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->drawIndexed(vertexArray); }

	private:
		static RendererAPI* s_RendererAPI;
	};
}
