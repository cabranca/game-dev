#pragma once

#include <Cabrankengine/Renderer/RendererAPI.h>

namespace cabrankengine {
	
	// RenderCommand is a class that provides a static interface for rendering commands.
	// Now it seems to just forward calls to the RendererAPI, but in the future the idea
	// is to be able to queue commands and execute them in another thread.
	class RenderCommand {
		public:
			// Initializes the RendererAPI. This should be called once at the start of the application.
			static void init() { s_RendererAPI->init(); }

			// Sets the color used to clear the screen.
			static void setClearColor(const glm::vec4& color) { s_RendererAPI->setClearColor(color); }

			// Clears the screen with the previously set clear color.
			static void clear() { s_RendererAPI->clear(); }

			// Draws the indexed vertices from the vertex array.
			static void drawIndexed(const Ref<VertexArray>& vertexArray) { s_RendererAPI->drawIndexed(vertexArray); }

		private:
			static RendererAPI* s_RendererAPI; // This is a pointer to the RendererAPI instance that will be used for rendering commands.
	};
}
