#include "Sandbox2D.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <Platform/OpenGL/OpenGLShader.h>

using namespace cabrankengine;

template<typename Fn>
	class Timer {
	public:
		Timer(const char* name, Fn&& func)
			: m_Name(name), m_Function(func), m_Stopped(false) {
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer() {
			if (!m_Stopped)
				stop();
		}

		void stop() {
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_Stopped = true;

			float duration = (end - start) * 0.001f;
			m_Function({ m_Name, duration });
		}

	private:
		const char* m_Name;
		Fn m_Function;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {}

void Sandbox2D::onAttach() {
	m_CheckerboardTexture = Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() {

}

void Sandbox2D::onUpdate(cabrankengine::Timestep delta) {
	PROFILE_SCOPE("Sandbox2D::onUpdate");

	// Update
	{
		PROFILE_SCOPE("CameraController::onUpdate");
		m_CameraController.onUpdate(delta);
	}

	// Render
	{
		PROFILE_SCOPE("Renderer Prep");
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Renderer2D::beginScene(m_CameraController.getCamera());

		Renderer2D::drawQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, m_SquareColor);
		Renderer2D::drawQuad({ 0.5f, 0.5f }, { 0.5f, 0.75f }, m_SquareColorVariant);
		Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);

		Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit4("Square Variant Color", glm::value_ptr(m_SquareColorVariant));

	for (const auto& result : m_ProfileResults) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::onEvent(cabrankengine::Event& e) {
	m_CameraController.onEvent(e);
}
