#pragma once

#include <pch.h>

#include <Cabrankengine/Math/Mat4.h>

#include "Lights.h"
#include "RendererAPI.h"

namespace cabrankengine::rendering {

	// Forward declarations
	class Camera; // Represents a camera used for rendering scenes.
	class Material;
	class Shader;
	class StorageBuffer;
	class VertexArray; // Represents a vertex array object that holds vertex data for rendering.

	// This is an abstracted API for rendering graphics in a game engine.
	class Renderer {
		public:
			// Initializes the renderer, setting up necessary resources and state.
			static void init();

			// Shuts down the renderer, releasing all resources and cleaning up state.
			static void shutdown();

			// Sets the necessary general data to render a scene, such as the camera, the lighting, etc.
			static void beginScene(const math::Mat4& projection, const math::Mat4& view, const LightEnvironment& environment);

			// Ends the current scene, finalizing rendering operations.
			static void endScene();

			// Submits a draw call to render a shader with a vertex array and an optional transformation matrix.
			static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const math::Mat4& transform);

			// Submits a draw call to render a shader with a vertex array and an optional transformation matrix.
			static void submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const math::Mat4& transform);

			// Sets the viewport dimensions for rendering.
			static void onWindowResize(uint32_t width, uint32_t height);

			// Returns the current API being used, allowing access to lower-level rendering functions.
			static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

		private:
			// This structure holds data that is specific to the current scene being rendered.
			struct SceneData {
				math::Mat4 projectionMatrix;
				math::Mat4 viewMatrix;
				LightEnvironment lightEnvironment;
				Ref<StorageBuffer> lightSSBO;
			};

			inline static SceneData* s_SceneData = new Renderer::SceneData(); // Current scene data used for rendering.

			static void uploadLightEnvironment();
	};
}
