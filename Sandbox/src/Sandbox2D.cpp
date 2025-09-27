#include "Sandbox2D.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <Cabrankengine/Core/AudioEngine.h>
#include <Cabrankengine/Debug/Instrumentator.h>
#include <Platform/OpenGL/OpenGLShader.h>

using namespace cabrankengine;
using namespace glm;

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(2560.0f, 1440.0f) {}

void Sandbox2D::onAttach() {
	CE_PROFILE_FUNCTION();

	m_CheckerboardTexture = Texture2D::create("assets/textures/Checkerboard.png");

	AudioEngine::playAudio("assets/sound/breakout.mp3", true);
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
		rotation += delta * 90.f;

		CE_PROFILE_SCOPE("Renderer Draw");
		//Renderer2D::beginScene(m_CameraController.getCamera());

		////Rotated red square
		//Renderer2D::drawRotatedQuad({ 640.f, 0.0f }, { 288.f, 288.f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		//
		////Red square
		//Renderer2D::drawQuad({ -640.f, 0.0f }, { 288.f, 288.f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//
		////Blue square
		//Renderer2D::drawQuad({ 320.f, -360.f }, { 180.f, 270.f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//
		////Checkerboard
		//Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 7200.0f, 7200.0f }, m_CheckerboardTexture, 10.0f);
		//
		////Rotated checkerboard
		//Renderer2D::drawRotatedQuad({ -1280.0f, 0.0f, 0.0f }, { 360.f, 360.f }, rotation, m_CheckerboardTexture, 20.0f);

		//Renderer2D::endScene();

		Renderer2D::beginScene(m_CameraController.getCamera());
		for (float y = -360.f; y < 360.f; y += 36.f) {
			for (float x = -360.f; x < 360.f; x += 36.f) {
				glm::vec4 color = { m_SquareColor };
				Renderer2D::drawQuad({ x, y }, { 30.f, 30.f }, color);
			}
		}
		Renderer2D::endScene();
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

	ImGui::ColorEdit4("Squares Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::onEvent(cabrankengine::Event& e) {
	m_CameraController.onEvent(e);
}
