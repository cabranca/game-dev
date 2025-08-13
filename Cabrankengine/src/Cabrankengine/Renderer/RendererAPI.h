#pragma once

#include <glm/glm.hpp>
#include <Cabrankengine/Core/Core.h>

namespace cabrankengine {

	class VertexArray; // Forward declaration of VertexArray class.

	// RendererAPI is an abstract class that defines the interface for the low level rendering operations.
	class RendererAPI {
		public:
			enum class API { None = 0, OpenGL = 1 }; // Enum representing the different rendering APIs supported.

			virtual ~RendererAPI() = default;

			// Initializes the renderer API. This method should be called before any rendering operations.
			virtual void init() = 0;

			// Sets the color used to clear the screen.
			virtual void setClearColor(const glm::vec4& color) = 0;

			// Clears the screen with the previously set clear color.
			virtual void clear() = 0;

			// Draws the indexed vertices from the vertex array.
			virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;

			// Sets the viewport dimensions for rendering.
			virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

			// Returns the current rendering API.
			static API getAPI() { return s_API; }

		private:
			inline static API s_API = RendererAPI::API::OpenGL; // Static variable that holds the current rendering API being used.
	};
}
