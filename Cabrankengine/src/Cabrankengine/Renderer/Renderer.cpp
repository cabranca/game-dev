#include <Cabrankengine/Renderer/Renderer.h>

#include <Cabrankengine/Renderer/OrthographicCamera.h>
#include <Cabrankengine/Renderer/Renderer2D.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/VertexArray.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace cabrankengine {

	void Renderer::init() {
		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::beginScene(OrthographicCamera& camera) {
		s_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene() {

	}

	void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjection", s_SceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_Transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::setViewport(0, 0, width, height);
	}
}
