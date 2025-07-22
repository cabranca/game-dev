#include <Cabrankengine/ImGui/ImGuiLayer.h>

#include <Platform/OpenGL/ImGuiOpenGLRenderer.h>
#include <GLFW/glfw3.h>
#include <Cabrankengine/Core/Application.h>
#include <Cabrankengine/Core/Window.h>

namespace cabrankengine {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer"), m_Time(0.f) {}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::onAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDettach()
	{

	}

	void ImGuiLayer::onUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.f / 60.f);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event& event)
	{

	}
}