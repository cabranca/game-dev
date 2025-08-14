#pragma once

#include <Cabrankengine/Renderer/RenderCommand.h>

namespace cabrankengine {

	// Forward declarations
	class OrthographicCamera; // Represents an orthographic camera used for rendering 2D scenes.
	class Shader; // Represents a shader program used for rendering.
	class VertexArray; // Represents a vertex array object that holds vertex data for rendering.

	// This is an abstracted API for rendering graphics in a game engine.
	class Renderer {
		public:
			// Initializes the renderer, setting up necessary resources and state.
			static void init();

			// Shuts down the renderer, releasing all resources and cleaning up state.
			static void shutdown();

			// Sets the necessary general data to render a scene, such as the camera, the lighting, etc.
			static void beginScene(OrthographicCamera& camera);

			// Ends the current scene, finalizing rendering operations.
			static void endScene();

			// Submits a draw call to render a shader with a vertex array and an optional transformation matrix.
			static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));

			// Sets the viewport dimensions for rendering.
			static void onWindowResize(uint32_t width, uint32_t height);

			// Returns the current API being used, allowing access to lower-level rendering functions.
			static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

		private:
			// This structure holds data that is specific to the current scene being rendered.
			struct SceneData {
				glm::mat4 viewProjectionMatrix;
			};

			inline static SceneData* s_SceneData = new Renderer::SceneData(); // Current scene data used for rendering.
	};
}
