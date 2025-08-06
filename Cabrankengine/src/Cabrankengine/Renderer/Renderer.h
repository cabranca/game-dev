#pragma once

#include <Cabrankengine/Renderer/RenderCommand.h>

namespace cabrankengine {

	class OrthographicCamera;
	class Shader;

	class Renderer {
	public:
		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f));

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}
