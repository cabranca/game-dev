#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Cabrankengine/Debug/Instrumentator.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include "Camera.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"


namespace cabrankengine {

	void Renderer::init() {
		CE_PROFILE_FUNCTION();

		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::shutdown() {
		Renderer2D::shutdown();
	}

	void Renderer::beginScene(const Camera& camera) {
		s_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene() {
		
	}

	void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const math::Mat4& transform) {
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("projection", s_SceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("view", translate(Vector3(0.0f, 0.0f, -3.0f)));
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("model", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::setViewport(0, 0, width, height);
	}
}
