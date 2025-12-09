#include "Renderer.h"

#include <Cabrankengine/Debug/Instrumentator.h>

#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"


namespace cabrankengine::rendering {

	using namespace math;

	void Renderer::init() {
		CE_PROFILE_FUNCTION();

		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::shutdown() {
		Renderer2D::shutdown();
	}

	void Renderer::beginScene(const math::Mat4& projection, const math::Mat4& view) {
		s_SceneData->projectionMatrix = projection;
		s_SceneData->viewMatrix = view;
	}

	void Renderer::endScene() {
		
	}

	void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const Mat4& transform) {
		shader->bind();
		shader->setMat4("projection", s_SceneData->projectionMatrix);
		shader->setMat4("view", s_SceneData->viewMatrix);
		shader->setMat4("model", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::setViewport(0, 0, width, height);
	}
}
