#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

using namespace cabrankengine;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::onAttach() {
	m_CheckerboardTexture = Texture2D::create("assets/textures/Checkerboard.png");
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

	Renderer2D::drawQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, m_SquareColor);
	Renderer2D::drawQuad({ 0.5f, 0.5f }, { 0.5f, 0.75f }, m_SquareColorVariant);
	Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);

	Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit4("Square Variant Color", glm::value_ptr(m_SquareColorVariant));

	ImGui::End();
}

void Sandbox2D::onEvent(cabrankengine::Event& e) {
	m_CameraController.onEvent(e);
}
