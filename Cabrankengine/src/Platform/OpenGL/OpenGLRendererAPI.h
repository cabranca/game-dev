#pragma once

#include <Cabrankengine/Renderer/RendererAPI.h>

namespace cabrankengine {

	class OpenGLRendererAPI : public RendererAPI {
		public:
			// Initializes the renderer API. This method should be called before any rendering operations.
			virtual void init() override;

			// Sets the color used to clear the screen.
			virtual void setClearColor(const glm::vec4& color) override;

			// Clears the screen with the previously set clear color.
			virtual void clear() override;

			// Draws the indexed vertices from the vertex array.
			virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;
			
			// Returns the current rendering API.
			static API getAPI() { return API::OpenGL; }
	};
}
