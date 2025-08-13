#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

using namespace cabrankengine;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::onAttach() {
	
}

void Sandbox2D::onDetach() {

}

void Sandbox2D::onUpdate(cabrankengine::Timestep delta) {
	// Update
	m_CameraController.onUpdate(delta);

	// Render
	RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::clear();

	Renderer2D::beginScene(m_CameraController.getCamera());

	Renderer2D::drawQuad({ 0.f, 0.f }, { 1.f, 1.f }, m_SquareColor);

	/*std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->bind();
	std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->uploadUniformFloat4("u_Color", m_SquareColor);*/

	Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::onEvent(cabrankengine::Event& e) {
	m_CameraController.onEvent(e);
}
