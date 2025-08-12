#include <Cabrankengine/Renderer/Renderer.h>

#include <Cabrankengine/Renderer/OrthographicCamera.h>
#include <Cabrankengine/Renderer/Shader.h>
#include <Cabrankengine/Renderer/VertexArray.h>
#include <Platform/OpenGL/OpenGLShader.h>

namespace cabrankengine {

	void Renderer::init() {
		RenderCommand::init();
	}

	void Renderer::beginScene(OrthographicCamera& camera) {
		s_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene() {

	}

	void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_viewProjection", s_SceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_Transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}
