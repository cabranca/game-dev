#include "Sandbox2D.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <Cabrankengine/Debug/Instrumentator.h>
#include <Platform/OpenGL/OpenGLShader.h>

using namespace cabrankengine;
using namespace glm;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {}

void Sandbox2D::onAttach() {
	CE_PROFILE_FUNCTION();

	m_CheckerboardTexture = Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() {
	CE_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(cabrankengine::Timestep delta) {
	CE_PROFILE_FUNCTION();

	// Update
	m_CameraController.onUpdate(delta);

	// Render
	Renderer2D::resetStats();
	{
		CE_PROFILE_SCOPE("Renderer Prep");
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();
	}

	{
		static float rotation = 0.f;
		rotation += delta * 50.f;

		CE_PROFILE_SCOPE("Renderer Draw");
		Renderer2D::beginScene(m_CameraController.getCamera());

		Renderer2D::drawRotatedQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, rotation, m_SquareColor);
		Renderer2D::drawQuad({ 0.5f, 0.5f }, { 0.5f, 0.75f }, m_SquareColorVariant);
		Renderer2D::drawQuad({ -5.f, -5.f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
		Renderer2D::drawRotatedQuad({ -5.f, -5.f, -0.1f }, { 1.0f, 1.0f }, 45.0f, m_CheckerboardTexture, 20.0f);

		Renderer2D::endScene();

		/*Renderer2D::beginScene(m_CameraController.getCamera());
		for (float y = -5.f; y < 5.f; y += 0.5f) {
			for (float x = -5.f; x < 5.f; x += 0.5f) {
				glm::vec4 color = { (x + 5.f) / 10.f, 0.4f, (y + 5.f) / 10.f, 1.0f };
				Renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Renderer2D::endScene();*/
	}
}

void Sandbox2D::onImGuiRender() {
	CE_PROFILE_FUNCTION();
	ImGui::Begin("Settings");

	auto stats = Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices %d:", stats.getTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::onEvent(cabrankengine::Event& e) {
	m_CameraController.onEvent(e);
}
